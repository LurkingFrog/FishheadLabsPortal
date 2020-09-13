//! The GraphQL queryable data set
//!
//! TODO: Convert this to be a Tyrell::Creche
//! THINK: Any good way to work around Input and Output objects needing to be different?
//!        https://github.com/graphql-rust/juniper/issues/173

use anyhow::{Context, Result};
use juniper::{graphql_value, FieldError, FieldResult};
use std::collections::HashMap;
use std::sync::{Arc, RwLock};
use uuid::Uuid;

pub mod errors;
pub mod models;
pub mod transforms;

pub use errors::CacheError;
use models::*;

// Enable us to use the ? operator on graphql objects
macro_rules! to_field_error {
  ($err:expr, $msg:expr) => {
    Err(FieldError::new(
      format!("{:#?}", $err),
      graphql_value!({ "internal_error": $msg }),
    ))
  };
}

/// A local, denormalized copy of the Fishhead Labs data
#[derive(Clone, Debug, Default)]
pub struct CacheData {
  organizations: HashMap<Uuid, Organization>,
  // This is a lookup based on Org Guid, for now.
  // TODO: Swap contacts with ContextMap<Organization, User>
  contacts: HashMap<Uuid, ContactMap>,
  addresses: HashMap<Uuid, Address>,

  // Can most likely turn this into a taskmaster for more rigid controls
  tasks: HashMap<Uuid, TaskInfo>,
}

impl std::fmt::Display for CacheData {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
    write!(f, "{:#?}", self)
  }
}

impl CacheData {
  pub fn new() -> CacheData {
    Default::default()
  }

  pub fn create_organization(&mut self, value: OrganizationInput) -> Result<Organization> {
    // Check if pretty ID exists
    // If yes, does the GUID match? DuplicateError if false
    let new_org = value.to_organization()?;
    self
      .organizations
      .insert(new_org.guid.clone(), new_org.clone());
    Ok(new_org)
  }
}

macro_rules! borrow_read {
  ($locked:expr) => {
    $locked.read().map_or_else(
      |err| {
        log::error!("Error getting a read lock on the database:\n{:#?}", err);
        Err(CacheError::DbWriteError)
      },
      |db| Ok(db),
    )
  };
}

macro_rules! borrow_write {
  ($locked:expr) => {
    $locked.write().map_or_else(
      |err| {
        log::error!("Error getting a write lock on the database:\n{:#?}", err);
        Err(CacheError::DbWriteError)
      },
      |db| Ok(db),
    )
  };
}

/// A local, denormalized copy of the Fishhead Labs data
#[derive(Clone, Debug, Default)]
pub struct Cache(Arc<RwLock<CacheData>>);
impl Cache {
  pub fn new() -> Cache {
    Default::default()
  }

  // CRUD for Tasks
  pub fn create_task(&self, task: TaskInfo) -> Result<TaskInfo> {
    let mut db = borrow_write!(self.0).context(format!(
      "Could not write the new task '{}' to the database",
      task.guid
    ))?;
    db.tasks.insert(task.guid.clone(), task.clone());
    Ok(task)
  }

  pub fn retrieve_task(&self, task_id: Uuid) -> Result<TaskInfo> {
    let db = borrow_read!(self.0).context(format!(
      "Could not search the database for task '{}'",
      task_id
    ))?;
    db.tasks.get(&task_id).map_or(
      Err(CacheError::NotFound).context(format!(
        "Task with ID '{}' was not found in the cache",
        task_id
      )),
      |x| Ok(x.clone()),
    )
  }

  // TODO: Turn this into a patch
  pub fn update_task(&self, task: TaskInfo) -> Result<TaskInfo> {
    let mut db = borrow_write!(self.0).context(format!(
      "Could not write the updated task '{}' to the database",
      task.guid
    ))?;

    let _previous = db.tasks.insert(task.guid.clone(), task.clone());
    Ok(task.clone())
  }

  // CRUD for Orgs
  pub fn create_organization(&self, _org_info: OrganizationInput) -> Result<Organization> {
    unimplemented!("'create_organization' still needs to be implemented")
  }
}

// A root schema consists of a query and a mutation.
// Request queries can be executed against a RootNode.
pub type CacheSchema = juniper::RootNode<'static, Query, Mutation>;

/// Store the CacheDatain a refcell for use with Juniper
// TODO: Create a generic juniper filter for querying the cache instead of individual items.
// TODO: Add CRUD macro
#[derive(Clone)]
pub struct JuniperCache {
  pub runtime: Arc<tokio::runtime::Runtime>,
  pub data: Cache,
}

impl juniper::Context for JuniperCache {}
impl JuniperCache {
  pub fn new(runtime: Arc<tokio::runtime::Runtime>) -> JuniperCache {
    JuniperCache {
      runtime,
      data: Cache::new(),
    }
  }

  pub fn get_schema() -> CacheSchema {
    CacheSchema::new(Query, Mutation)
  }

  pub fn new_task(&self, task_name: String) -> Result<TaskInfo> {
    let task_info = TaskInfo::new(task_name);
    self.data.create_task(task_info)
  }

  pub fn get_task(&self, task_id: Uuid) -> Result<TaskInfo> {
    self.data.retrieve_task(task_id)
  }

  pub fn update_task(&self, _task_info: TaskInfo) -> Result<()> {
    unimplemented!("'update_task' still needs to be implemented")
  }
}

pub struct Query;

#[juniper::object(
  // Here we specify the context type for the object.
  // We need to do this in every type that
  // needs access to the context.
  Context = JuniperCache,
)]
impl Query {
  fn version() -> &str {
    "1.0"
  }

  fn get_orgs(context: &JuniperCache) -> FieldResult<Vec<Organization>> {
    let result = borrow_read!(context.data.0)?
      .organizations
      .values()
      .into_iter()
      .map(|x| x.clone())
      .collect();
    // Return the result.
    Ok(result)
  }
}

pub struct Mutation;

#[juniper::object(
    Context = JuniperCache,
)]
impl Mutation {
  fn import_workbook(
    context: &JuniperCache,
    input: transforms::google_sheets::ImportWorkbook,
  ) -> FieldResult<TaskInfo> {
    let task_info = context.new_task(format!("Importing workbook '{}'", input.sheet_id));
    match task_info {
      Err(err) => to_field_error!(err, "Could not create a new Import Workbook task"),
      Ok(task_info) => context.runtime.enter(|| {
        let db = context.data.clone();
        let task_id = task_info.guid.clone();
        tokio::spawn(async move { input.run_import(db, task_id).await });
        Ok(task_info)
      }),
    }
  }

  fn createOrganization(
    context: &JuniperCache,
    input: OrganizationInput,
  ) -> FieldResult<Organization> {
    context.data.create_organization(input.clone()).map_or_else(
      |err| to_field_error!(err, "There was a probelm creating the new organization"),
      |x| Ok(x),
    )
  }
}

/*
mutation Importer($input: ImportWorkbook!) {
  importWorkbook(input: $input) {
    guid
    state
    name
    status
    errors
  }
}

{
  "input": {
    "sheetId": "1Y-swLK2kje-BSFIXIVNQNejNsLO8GamTST9LmZsPwLI",
    "sheetNames": []
  }
}
*/
