//! Track the state of long running processes

use super::CacheError;
use uuid::Uuid;

pub trait TaskStatus: std::fmt::Display + std::fmt::Debug + juniper::GraphQLType {}

#[derive(juniper::GraphQLObject, Clone, Debug, Default)]
pub struct TaskInfo {
  pub guid: Uuid,
  pub state: TaskState,
  pub name: String,
  // status: Option<Rc<dyn TaskStatus>>,
  pub status: Vec<String>,
  pub errors: Vec<CacheError>,
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
      status: vec![],
      errors: vec![],
    }
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
