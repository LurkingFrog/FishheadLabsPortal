//! Managing contact methods
//!
//! There are always multiple ways to contact a organization/person, and how it is done depends upon the
//! context (or reason) it it being done. For example, for non-urgent items one might send an email while for
//! an immediate response it is faster to make a phone call".to_string()
//!
//! THINK: THis is a rabbit hole I shouldn't go too deeply down yet. Just want to be able to map basic
//!        phone numbers, emails, and addresses to organizations/users

use serde::{Deserialize, Serialize};
use uuid::Uuid;

/// Methods of contacting a person/organization based on the context they are being contacted.
///
/// FIXME: This will be deprecated, as contexts are properly implemented
/// This should give a number of filters on being able to find who and how to contact a person based upon
/// the reason for the contact. If I want to follow up on a payment issue, many times its going to be a
/// different person than the person who ordered the product. The priority is to list the best places to
/// try (think calling an office line, then reception, and finally a cell)
#[derive(Clone, Eq, PartialEq, Ord, PartialOrd, Hash, Debug, Default, Serialize, Deserialize)]
pub struct ContactMap {
  /// The unique id of this contact
  pub guid: Uuid,
  /// The order in which these should be attempted. Lowest first and equal numbers will be picked at random
  pub priority: i8,
  /// The reason for the contact. For example, the mailing address may be different
  /// from the billing address and shipping address.
  pub context: String,
  /// Any additional information that may be linked to the context of this given contact such as delivery
  /// instructions or best times to call
  pub description: Option<String>,
  /// The organization
  pub org_guid: Option<Uuid>,
  /// The person using this address
  pub user_guid: Option<Uuid>,
  /// The ID of the physical address
  pub address_guid: Option<Uuid>,
  /// The main phone number
  pub phone_number: Option<String>,
  /// The main email address
  pub email_address: Option<String>,
}

impl std::fmt::Display for ContactMap {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
    write!(f, "{:#?}", self)
  }
}

impl ContactMap {}

/// THis
#[derive(Clone, Eq, PartialEq, Ord, PartialOrd, Hash, Debug, Default, Serialize, Deserialize)]
pub struct ContactContext {}

impl std::fmt::Display for ContactContext {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
    write!(f, "{:#?}", self)
  }
}

impl ContactContext {}

/// A wrapper for all the ways one can be contacted
#[derive(Clone, Eq, PartialEq, Ord, PartialOrd, Hash, Debug, Serialize, Deserialize)]
pub enum ContactMethod {
  Phone(Uuid),
  Email(Uuid),
  Address(Uuid),
}

impl Default for ContactMethod {
  fn default() -> ContactMethod {
    unimplemented!("Default is not implemented for ContactMethod")
  }
}
