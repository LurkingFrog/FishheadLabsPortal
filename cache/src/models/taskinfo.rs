//! Track the state of long running processes

use anyhow::{Context, Result};
use chrono::{DateTime, Utc};
use serde::{Deserialize, Serialize};
use uuid::Uuid;

use super::CacheError;

pub trait TaskStatus<'de>: Default + Deserialize<'de> + Serialize {}

#[derive(juniper::GraphQLObject, Clone, Debug)]
pub struct TaskInfo {
  pub guid: Uuid,
  pub state: TaskState,
  pub last_modified: DateTime<Utc>,
  pub name: String,
  // status: Option<Rc<dyn TaskStatus>>,
  pub status: String,
}

impl std::fmt::Display for TaskInfo {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
    write!(f, "{:#?}", self)
  }
}

impl Default for TaskInfo {
  fn default() -> TaskInfo {
    TaskInfo {
      guid: Uuid::new_v4(),
      state: TaskState::Pending,
      last_modified: Utc::now(),
      name: Default::default(),
      status: Default::default(),
    }
  }
}

impl TaskInfo {
  pub fn new(name: String) -> TaskInfo {
    TaskInfo {
      name,
      ..Default::default()
    }
  }

  pub fn from_juniper_value(value: juniper::Value) -> Result<TaskInfo> {
    let values = value.as_object_value().unwrap();
    let guid = values.get_field_value("guid").map_or(
      Err(CacheError::NotFound).context(format!("'guid' field was not found in Graphql object")),
      |guid| {
        guid.as_scalar_value::<String>().map_or(
          Err(CacheError::ConversionError).context(format!("'guid' GraphQL TaskInfo")),
          |value| {
            Uuid::parse_str(value).map_err(|err| {
              anyhow::Error::new(CacheError::ConversionError).context(format!(
                "'guid' field could not be converted to a Uuid:\n{:#?}",
                err
              ))
            })
          },
        )
      },
    )?;

    let state = values.get_field_value("state").map_or(
      Err(CacheError::NotFound).context(format!("'state' field was not found in Graphql object")),
      |guid| {
        guid.as_scalar_value::<String>().map_or(
          Err(CacheError::ConversionError).context(format!("'state' GraphQL TaskInfo")),
          |value| TaskState::parse_str(value),
        )
      },
    )?;

    let name = values
      .get_field_value("name")
      .map_or(
        Err(CacheError::NotFound).context(format!("'name' field was not found in Graphql object")),
        |name| {
          name.as_scalar_value::<String>().ok_or(
            anyhow::Error::new(CacheError::ConversionError)
              .context(format!("'name' GraphQL TaskInfo")),
          )
        },
      )?
      .clone();

    let status = values
      .get_field_value("status")
      .map_or(
        Err(CacheError::NotFound)
          .context(format!("'status' field was not found in Graphql object")),
        |name| {
          name.as_scalar_value::<String>().ok_or(
            anyhow::Error::new(CacheError::ConversionError)
              .context(format!("'status' GraphQL TaskInfo")),
          )
        },
      )?
      .clone();

    let last_modified = values
      .get_field_value("last_modified")
      .map_or(
        Err(CacheError::NotFound).context(format!(
          "'last_modified' field was not found in Graphql object"
        )),
        |timestamp| {
          timestamp.as_scalar_value::<String>().map_or(
            Err(CacheError::ConversionError).context(format!("'last_modified' GraphQL TaskInfo")),
            |value| {
              value.parse::<DateTime<Utc>>().map_err(|_| {
                anyhow::Error::new(CacheError::ConversionError)
                  .context(format!("Could not make a Date/Time from '{}'", value))
              })
            },
          )
        },
      )?
      .clone();

    Ok(TaskInfo {
      guid,
      name,
      state,
      last_modified,
      status,
    })
  }

  pub fn set_status<'a>(&mut self, status: &impl TaskStatus<'a>) -> Result<()> {
    self.status = serde_json::to_string(&status).context(format!(
      "Could not serialize the status for TaskInfo '{}'",
      self.guid
    ))?;
    Ok(())
  }
}

#[derive(juniper::GraphQLScalarValue, Clone, Debug)]
pub struct Timestamp(DateTime<Utc>);

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

impl TaskState {
  pub fn parse_str(value: &String) -> Result<TaskState> {
    Ok(match &value.to_lowercase()[..] {
      "pending" => TaskState::Pending,
      "running" => TaskState::Running,
      "paused" => TaskState::Paused,
      "blocked" => TaskState::Blocked,
      "error" => TaskState::Error,
      "done" => TaskState::Done,
      _ => Err(CacheError::InvalidValue)
        .context(format!("'{}' is not a valid value for TaskState", value))?,
    })
  }
}

impl Default for TaskState {
  fn default() -> TaskState {
    unimplemented!("Default is not implemented for TaskState")
  }
}
