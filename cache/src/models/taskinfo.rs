//! Track the state of long running processes

use anyhow::{Context, Result};
use serde::{Deserialize, Serialize};
use uuid::Uuid;

use super::CacheError;

pub trait TaskStatus<'de>: Default + Deserialize<'de> + Serialize {}

#[derive(juniper::GraphQLObject, Clone, Debug, Default)]
pub struct TaskInfo {
  pub guid: Uuid,
  pub state: TaskState,
  pub name: String,
  // status: Option<Rc<dyn TaskStatus>>,
  status: String,
}

impl std::fmt::Display for TaskInfo {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
    write!(f, "{:#?}", self)
  }
}

impl TaskInfo {
  pub fn new(name: String) -> TaskInfo {
    TaskInfo {
      guid: Uuid::new_v4(),
      state: TaskState::Pending,
      name: name,
      status: Default::default(),
    }
  }

  pub fn set_status<'a>(&mut self, status: impl TaskStatus<'a>) -> Result<()> {
    self.status = serde_json::to_string(&status).context(format!(
      "Could not serialize the status for TaskInfo '{}'",
      self.guid
    ))?;
    Ok(())
  }
}

///
#[derive(juniper::GraphQLEnum, Clone, Eq, PartialEq, Ord, PartialOrd, Hash, Debug)]
pub enum TaskState {
  Pending,
  Running,
  Paused,
  Blocked,
  Error,
  Done,
}

impl Default for TaskState {
  fn default() -> TaskState {
    unimplemented!("Default is not implemented for TaskState")
  }
}
