//! The GraphQL queryable data set
//!
//! TODO: Convert this to be a Tyrell::Creche
//! THINK: Any good way to work around Input and Output objects needing to be different?
//!        https://github.com/graphql-rust/juniper/issues/173

use anyhow::Result;
use juniper::{graphql_value, FieldError, FieldResult};
use std::collections::HashMap;
use std::sync::{Arc, Mutex};
use uuid::Uuid;

pub mod errors;
pub mod models;
pub mod transforms;

pub use errors::CacheError;
use models::*;
use transforms::*;

// Enable us to use the ? operator on graphql objects
macro_rules! to_field_error {
  ($err:expr, $msg:expr) => {
    Err(FieldError::new(
      format!("{:#?}", $err),
      graphql_value!({ "internal_error": $msg }),
    ))
  };
}

pub trait CacheTransform {
  fn import(&self, cache: Option<Cache>) -> Result<()>;
  fn export(&self, cache: Cache) -> Result<()>;
}

/// A local, denormalized copy of the Fishhead Labs data
#[derive(Clone, Debug, Default)]
pub struct Cache {
  organizations: HashMap<Uuid, Organization>,
  // This is a lookup based on Org Guid, for now
  contacts: HashMap<Uuid, ContactMap>,
  addresses: HashMap<Uuid, Address>,
  tasks: HashMap<Uuid, TaskInfo>,
}

impl std::fmt::Display for Cache {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
    write!(f, "{:#?}", self)
  }
}

impl Cache {
  pub fn new() -> Cache {
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

// A root schema consists of a query and a mutation.
// Request queries can be executed against a RootNode.
pub type CacheSchema = juniper::RootNode<'static, Query, Mutation>;

/// Store the cache in a refcell for use with Juniper
#[derive(Clone)]
pub struct JuniperCache {
  pub runtime: Arc<tokio::runtime::Runtime>,
  pub data: Arc<Mutex<Cache>>,
}

impl juniper::Context for JuniperCache {}
impl JuniperCache {
  pub fn new(runtime: Arc<tokio::runtime::Runtime>) -> JuniperCache {
    JuniperCache {
      runtime,
      data: Arc::new(Mutex::new(Cache::new())),
    }
  }

  pub fn get_schema() -> CacheSchema {
    CacheSchema::new(Query, Mutation)
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
    let result = context
      .data
      .lock()
      .unwrap()
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
    input: transforms::sheets::ImportWorkbook,
  ) -> FieldResult<TaskInfo> {
    let mut task_info = TaskInfo::new("import_workbook".to_string());
    context.runtime.enter(|| {
      let db = context.data.clone();
      let task_id = task_info.guid.clone();
      tokio::spawn(async move {
        input.run_import(db, task_id);
      })
    });

    Ok(task_info)
  }

  fn createOrganization(
    context: &JuniperCache,
    input: OrganizationInput,
  ) -> FieldResult<Organization> {
    let mut cache = context.data.lock().unwrap();
    let new_org = cache.create_organization(input.clone());

    match new_org {
      Ok(x) => Ok(x),
      Err(err) => to_field_error!(err, "There was a probelm creating the new organization"),
    }
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
