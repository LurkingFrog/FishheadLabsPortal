//! A group of people coupled with business functions
//!
//! This can be anything from a single person up to a multi-national corporation. This is used for defining
//! ownership. In financial terms, this is a cost center that can be billed and pay.
//!
//! This is designed to be recursive, with a context sensitive overriding of configuration.

use super::CacheError;
use anyhow::{Context, Result};
use serde::{Deserialize, Serialize};
use uuid::Uuid;

#[derive(juniper::GraphQLObject, Clone, Eq, PartialEq, Debug, Default, Serialize, Deserialize)]
/// A group with a common business function
pub struct Organization {
  pub guid: Uuid,

  /// A mapping of how other organizations may refer to this one.
  pub external_id: Option<String>,
  pub pretty_id: Option<String>,
  pub legal_name: String,
  pub parent: Option<Uuid>,
  pub children: Vec<Uuid>,
  pub config: Option<OrganizationConfig>,
}

impl std::fmt::Display for Organization {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
    write!(f, "{:#?}", self)
  }
}

impl Organization {
  pub fn new(guid: Uuid, legal_name: String) -> Result<Organization> {
    Ok(Organization {
      guid,
      legal_name,
      ..Default::default()
    })
  }
}

#[derive(
  juniper::GraphQLObject,
  Clone,
  Eq,
  PartialEq,
  Ord,
  PartialOrd,
  Hash,
  Debug,
  Default,
  Serialize,
  Deserialize,
)]
pub struct OrganizationConfig {}

impl std::fmt::Display for OrganizationConfig {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
    write!(f, "{:#?}", self)
  }
}

impl OrganizationConfig {}

#[derive(
  juniper::GraphQLInputObject,
  Clone,
  Eq,
  PartialEq,
  Ord,
  PartialOrd,
  Hash,
  Debug,
  Default,
  Serialize,
  Deserialize,
)]
pub struct OrganizationInput {
  pub guid: Option<Uuid>,
  pub external_id: Option<String>,
  pub pretty_id: Option<String>,
  pub legal_name: Option<String>,
  pub parent: Option<Uuid>,
  pub children: Option<Vec<Uuid>>,
}

impl std::fmt::Display for OrganizationInput {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
    write!(f, "{:#?}", self)
  }
}

impl OrganizationInput {
  pub fn to_organization(&self) -> Result<Organization> {
    let guid = self.guid.unwrap_or_else(|| uuid::Uuid::new_v4()).to_owned();

    let legal_name = self
      .legal_name
      .clone()
      .ok_or(Err(CacheError::RequiredField).context("New organizations require 'legal_name'")?)
      .unwrap();

    Ok(Organization {
      external_id: self.external_id.to_owned(),
      pretty_id: self.pretty_id.to_owned(),
      parent: self.parent.to_owned(),
      children: self.children.to_owned().unwrap_or_default(),
      ..Organization::new(guid, legal_name)?
    })
  }
}
