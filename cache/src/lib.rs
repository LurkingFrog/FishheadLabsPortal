//! The GraphQL queryable data set
//!
//! TODO: Convert this to be a Tyrell::Creche
//! THINK: Any good way to work around Input and Output objects needing to be different?
//!        https://github.com/graphql-rust/juniper/issues/173

use anyhow::Result;
use juniper::{graphql_value, FieldError, FieldResult};
use std::cell::RefCell;
use std::collections::HashMap;
use uuid::Uuid;

pub mod errors;
pub mod models;

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
pub struct JuniperCache(RefCell<Cache>);
impl juniper::Context for JuniperCache {}
impl JuniperCache {
  pub fn new() -> JuniperCache {
    JuniperCache(RefCell::new(Cache::new()))
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
      .0
      .borrow()
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
  fn createOrganization(
    context: &JuniperCache,
    input: OrganizationInput,
  ) -> FieldResult<Organization> {
    let new_org = context.0.borrow_mut().create_organization(input.clone());

    match new_org {
      Ok(x) => Ok(x),
      Err(err) => to_field_error!(err, "There was a probelm creating the new organization"),
    }
    // {
    // Err(err) => to_field_error!(
    //   err,
    //   "The organizations in the cache are unavailable for insert, please retry later"
    // ),
    // Ok(x) => x
  }
}
