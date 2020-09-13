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
  // THINK: Permissions on the task_info: should it be locked so only this process can update it?
  pub async fn run_import(&self, db: Cache, task_id: Uuid) -> Result<()> {
    log::warn!("Running the import");
    let mut task_status = ImportWorkbookStatus::new();
    let mut task_info = db.retrieve_task(task_id)?;

    // Update task to Running with an empty state
    task_info.state = TaskState::Running;
    task_info.set_status(&task_status)?;
    db.update_task(task_info.clone())
      .context("Failed to update task")?;

    // TODO: Link the credentials to the user. Look into OAuth2 login to get token from google
    let creds = "/project/fhl_service_acct.json";

    // Update task info to "Importing **Metadata**
    let wb = sheets::open_workbook(creds, &self.sheet_id)?;
    wb.metadata.sheet_map.get("organizations").map_or(
      Err(CacheError::NotFound).context(format!(
        "Sheet 'organizations' not found in workbook '{}'",
        wb.metadata.name
      )),
      |metadata| {
        task_status.organizations.total_rows = metadata.range.0.clone() as i32;
        Ok(())
      },
    )?;

    task_info.set_status(&task_status)?;
    db.update_task(task_info.clone())
      .context("Failed to update task")?;
    log::debug!("wb.metadata:\n{:#?}", task_info);

    // Pull Organizations from Sheets

    // Update Task info "Read x Orgs of y"

    //

    // Task Info Steps:
    //

    log::warn!("Done running the importer");
    Ok(())
  }
}

#[derive(Clone, Debug, Default, Serialize, Deserialize)]
pub struct ImportWorkbookStatus {
  organizations: WorksheetStatus,
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

/*
  organization ->
    to_read ->
    read ->
    processed ->
*/

#[derive(Clone, Debug, Default, Serialize, Deserialize)]
pub struct WorksheetStatus {
  total_rows: i32,
  read: i32,
  processed: i32,
  skipped: i32,
  errors: Vec<CacheError>,
}

impl std::fmt::Display for WorksheetStatus {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
    write!(f, "{:#?}", self)
  }
}

impl WorksheetStatus {}

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
