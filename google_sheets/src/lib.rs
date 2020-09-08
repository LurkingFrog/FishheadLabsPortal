//! Google Docs data layout for FishheadLabs
//!
//! This is the interface to the spreadsheet form of the Fishhead Labs data. It uses subpar to import the data
//! but there currently isn't any update functionality. This is a load of each worksheet into an vector of
//! objects.

//! Read and write from the Submission Log in Google Sheets

#[derive(Clone, Eq, PartialEq, Ord, PartialOrd, Hash, Debug, Default)]
pub struct SheetsImporter {}

impl std::fmt::Display for SheetsImporter {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
    write!(f, "{:#?}", self)
  }
}

impl SheetsImporter {}

use anyhow::{Context, Result};

use subpar::{ExcelObject, MetaWorkbook, SubparTable};

pub fn get_workbook(creds: &str, sheet_id: &str) -> Result<FHLWorkbook> {
  log::debug!("Loading a workbook");
  let db_conf = subpar::WorkbookConfig::new_sheets_config(
    Some(sheet_id.to_string()),
    creds.to_string(),
    "fhl@landfillinc.com".to_string(),
  );
  let wb = subpar::Workbook::open(&db_conf)
    .context(format!("Could not get the workbook '{}'", sheet_id))?;
  Ok(FHLWorkbook::from_excel(&ExcelObject::Workbook(wb.clone()))?)
}

/// A composite db of all the sheets in the workbook
#[derive(SubparTable, Clone, Debug)]
pub struct FHLWorkbook {
  pub organizations: Vec<Organization>,
  // users: Vec<models::User>,
  // submissions: Vec<models::Submission>,
  // histology_submissions: Vec<models::HistologySubmission>,
}

impl std::fmt::Display for FHLWorkbook {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
    write!(
      f,
      // "\tsubmissions: {}\n\tusers: {}\n\torganizations: {}\n\thistology_submissions: {}",
      "\torganizations: {}",
      // self.submissions.len(),
      // self.users.len(),
      self.organizations.len(),
      // self.histology_submissions.len()
    )
  }
}

/// A helper function used to partially deal with embedded JSON.
///
/// This is used in line items in a
fn cell_csv_to_vec(column: &str, wrapped: &ExcelObject) -> Result<Vec<String>> {
  let row = wrapped
    .unwrap_row()
    .expect("There was an error unwrapping the object in cell_csv_to_vec");
  match row.get(column) {
    None => panic!(
      "No cell named '{}' in the row. Options are: {:#?}",
      column,
      row.keys()
    ),
    Some(cell) => match &cell.data {
      subpar::CellType::String(value) => Ok(
        value
          .trim_matches(|c| c == '[' || c == ']')
          .replace(" ", "")
          .split(",")
          .filter(|&x| x != "")
          .map(|s| s.to_string())
          .collect::<Vec<_>>(),
      ),
      x => panic!(format!(
        "\n!!! Cannot turn {:?} into a Vec<String> for '{}'",
        x, column
      )),
    },
  }
}

/// Helper parser to extract a list of users belonging to an organization
///
/// This should likely become mapped to a many-to-many role
/// Denise Petty is a good example, submitting from half a dozen different orgs
fn parse_members(wrapped: &ExcelObject) -> Result<Vec<i32>> {
  let members = cell_csv_to_vec("Members", wrapped)?;
  let result = vec![];
  members.iter().fold(Ok(result), |acc, member| match acc {
    Err(_) => acc,
    Ok(mut x) => match member.parse::<i32>() {
      Err(err) => Err(err)?,
      Ok(y) => {
        x.extend(vec![y]);
        Ok(x)
      }
    },
  })
}

fn parse_children(wrapped: &ExcelObject) -> Result<Vec<i32>> {
  let children = cell_csv_to_vec("Children", wrapped)?;
  let result = vec![];
  children.iter().fold(Ok(result), |acc, child| match acc {
    Err(_) => acc,
    Ok(mut x) => match child.parse::<i32>() {
      Err(err) => Err(err)?,
      Ok(y) => {
        x.extend(vec![y]);
        Ok(x)
      }
    },
  })
}

#[derive(Clone, Debug, SubparTable)]
pub struct Organization {
  #[subpar(rename = "Guid")]
  pub guid: i32,
  #[subpar(rename = "Pretty_ID")]
  pub pretty_id: String,
  #[subpar(rename = "Name")]
  pub name: String,
  #[subpar(rename = "Department")]
  pub department: Option<String>,
  #[subpar(rename = "Email")]
  pub email: Option<String>,
  #[subpar(rename = "Phone")]
  pub phone: Option<String>,
  #[subpar(rename = "Address1")]
  pub address1: String,
  #[subpar(rename = "Address2")]
  pub address2: Option<String>,
  #[subpar(rename = "City")]
  pub city: String,
  #[subpar(rename = "State")]
  pub state: String,
  #[subpar(rename = "Zip")]
  pub zip: String,
  #[subpar(rename = "Parent_ID")]
  pub parent_id: Option<i32>,
  #[subpar(parser = "parse_members")]
  pub members: Vec<i32>,
  #[subpar(parser = "parse_children")]
  pub children: Vec<i32>,
  #[subpar(rename = "Payment_Terms")]
  pub payment_terms: String,
}
