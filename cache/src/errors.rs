//! Errors that can be generated by the cache
//!
//! We rewrap all the modules errors so the end coder can catch a fully enumerated list of errors

use thiserror::Error;

#[derive(juniper::GraphQLEnum, Debug, Clone, Error)]
pub enum CacheError {
  #[error("There was an error attempting to convert from one type to another")]
  ConversionError,

  #[error("Tried to borrow a locked value for mutation")]
  BorrowError,

  #[error("Input item received a none for a non-optional field in a create")]
  RequiredField,
}
