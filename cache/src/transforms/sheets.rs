//! Google Docs data layout
//!
//! This is the interface to the spreadsheet form of the Fishhead Labs data. It uses subpar to import the data
//! but there currently isn't any update functionality.
use std::sync::{Arc, Mutex};
use uuid::Uuid;

use super::{Cache, TaskInfo};
use anyhow::{Context, Result};

#[derive(
  juniper::GraphQLInputObject, Clone, Eq, PartialEq, Ord, PartialOrd, Hash, Debug, Default,
)]
pub struct ImportWorkbook {
  sheet_id: String,
  sheet_names: Option<Vec<String>>,
}

impl std::fmt::Display for ImportWorkbook {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
    write!(f, "{:#?}", self)
  }
}

impl ImportWorkbook {
  // pub fn import_workbook(
  //   &self,
  //   creds_path: String,
  //   cache: super::JuniperCache,
  // ) -> Result<TaskInfo> {
  //   // Check creds path

  //   // Ping sheet, get base info?
  //   // Create Task Info

  //   // Spawn task

  //   // Return Task Info
  //   let mut task_info = TaskInfo::new("import_workbook".to_string());

  //   // Insert it into the database

  //   // Spawn the importer
  //   let cloned = cache.clone();
  //   tokio::spawn(async move {
  //     self.run_importer(&cloned);
  //   });

  //   unimplemented!("'import_workbook' still needs to be implemented")
  // }

  pub fn run_import(&self, cache: Arc<Mutex<Cache>>, task: Uuid) -> () {
    log::debug!("Running the import");
    log::error!("'run_importer' still needs to be implemented")
  }
}

// pub fn get_address() -> Result<Address> {
//   Address::new_us(
//     Some(self.name.clone()),
//     self.address1.clone(),
//     self.address2.clone(),
//     self.city.clone(),
//     self.state.clone(),
//     self.zip.clone(),
//   )
// }
