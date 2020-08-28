//! Mailing address functionality
//!
//! This is a separate file as validation, standardization, and shipping functionality will be built on top
//! of this library.

use anyhow::Result;
use serde::{Deserialize, Serialize};
use uuid::Uuid;

/// A standard USPS postal address mapping to a single location
///
/// This is done to the USPS standard, since if it validates, it should be shippable. The validation is
/// coming later and will likely be spun off into it's own item".to_string()
/// https://www.usps.com/business/web-tools-apis/address-information-api.htm#_Toc39492054
///
/// The standardized addresses are going to be static and will not be able to be changed in the database,
/// since multiple parents can point to the same address. If one parent moves, they won't take the address
/// with them (though because the firm name is in there it may not be able to be reused)
/// TODO: Validate and standardize using USPS or something like https://smartystreets.com/
/// TODO: Spin off into a full blown external module that can be used by USPS, UPS, and FedEx to create labels
#[derive(Clone, Eq, PartialEq, Ord, PartialOrd, Hash, Debug, Default, Serialize, Deserialize)]
pub struct Address {
  /// The building/firm being addressed. (this field is not intended to store an individual person)
  addressee: Option<String>,
  address1: String,
  address2: Option<String>,
  address3: Option<String>,
  city: Option<String>,
  state: Option<String>,
  urbanization: Option<String>,
  zip: Option<String>,
  country: String,
}

impl std::fmt::Display for Address {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
    write!(f, "{:#?}", self)
  }
}

/// Options for creating a new address (totally unimplemented, future iterations for this)
#[derive(Clone, Eq, PartialEq, Ord, PartialOrd, Hash, Debug, Default, Serialize, Deserialize)]
pub struct AddressOptions {
  // Make sure that the address conforms to the country and that it survives any lookups
// validate: bool,

// Reformats the fields into the USPS standard (unimplemented)
// standardize: bool,
}

impl std::fmt::Display for AddressOptions {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
    write!(f, "{:#?}", self)
  }
}

impl AddressOptions {}

impl Address {
  pub fn new(
    addressee: Option<String>,
    address1: String,
    address2: Option<String>,
    address3: Option<String>,
    city: Option<String>,
    state: Option<String>,
    urbanization: Option<String>,
    zip: Option<String>,
    country: String,
    _opts: AddressOptions,
  ) -> Result<Address> {
    // Attempt to standardize if bool is true

    Ok(Address {
      addressee,
      address1,
      address2,
      address3,
      city,
      state,
      urbanization,
      zip,
      country,
    })
  }

  /// Create a new simplified, US address
  ///
  /// This excludes Puerto Rico due to their use of the urbanization field. If you need that, please use
  ///
  pub fn new_us(
    addressee: Option<String>,
    address1: String,
    address2: Option<String>,
    city: String,
    state: String,
    zip: String,
  ) -> Result<Address> {
    Address::new(
      addressee,
      address1,
      address2,
      None,
      Some(city),
      Some(state),
      None,
      Some(zip),
      "US".to_string(),
      Default::default(),
    )
  }

  /// Create a Uuid using a sha1 hash of all the address values
  pub fn make_guid(&self) -> Uuid {
    let value = [
      self.addressee.clone(),
      Some(self.address1.clone()),
      self.address2.clone(),
      self.address3.clone(),
      self.city.clone(),
      self.state.clone(),
      self.urbanization.clone(),
      self.zip.clone(),
      Some(self.country.clone()),
    ]
    .iter()
    .filter(|x| x.is_some())
    .map(|x| x.clone().unwrap())
    .collect::<Vec<String>>();
    // .join("")
    // .as_bytes();
    Uuid::new_v5(&Uuid::NAMESPACE_OID, value.join("").as_bytes())
  }
}
