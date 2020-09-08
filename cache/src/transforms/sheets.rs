//! Google Docs data layout
//!
//! This is the interface to the spreadsheet form of the Fishhead Labs data. It uses subpar to import the data
//! but there currently isn't any update functionality.

use serde::{Deserialize, Serialize};
use uuid::Uuid;

use super::{Cache, CacheError, TaskState, TaskStatus};
use anyhow::{Context, Result};

#[derive(
  juniper::GraphQLInputObject, Clone, Eq, PartialEq, Ord, PartialOrd, Hash, Debug, Default,
)]
pub struct ImportWorkbook {
  pub sheet_id: String,
  pub sheet_names: Option<Vec<String>>,
}

impl std::fmt::Display for ImportWorkbook {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
    write!(f, "{:#?}", self)
  }
}

impl ImportWorkbook {
  pub async fn run_import(&self, db: Cache, task_id: Uuid) -> Result<()> {
    log::warn!("Running the import");
    let task_status = ImportWorkbookStatus::new();
    let mut task_info = db.retrieve_task(task_id)?;

    // Update task to Running with an empty state
    task_info.state = TaskState::Running;
    task_info.set_status(task_status)?;
    db.update_task(task_info.clone())?;

    // Get Metadata from Workbook

    // Update task info to "Importing **Metadata**

    // Pull Organizations from Sheets

    // Update Task info "Read x Orgs of y"

    //

    // Task Info Steps:
    //

    log::debug!("Got the task info {:#?}", task_info);

    log::warn!("Done running the importer");
    Ok(())
  }
}

#[derive(Clone, Debug, Default, Serialize, Deserialize)]
pub struct ImportWorkbookStatus {
  metadata: String,
  errors: Option<CacheError>,
}

impl std::fmt::Display for ImportWorkbookStatus {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
    write!(f, "{:#?}", self)
  }
}

impl<'a> TaskStatus<'a> for ImportWorkbookStatus {}

impl ImportWorkbookStatus {
  pub fn new() -> ImportWorkbookStatus {
    Default::default()
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
