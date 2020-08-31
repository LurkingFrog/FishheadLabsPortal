//! The data structure
//!
//! This is a slightly denormalized data structure for storing all the information that is used by the
//! Fishhead Labs portal.
//!

// https://doc.rust-lang.org/reference/conditional-compilation.html
// #[cfg_attr(feature = "graphql_enabled", derive(Serialize, Deserialize))]

pub mod address;
pub mod contact;
pub mod organization;
pub mod taskinfo;

pub use address::*;
pub use contact::*;
pub use organization::*;
pub use taskinfo::*;

use super::CacheError;
