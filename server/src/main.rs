//! The primary warp data server
//!
//! This is the backend combining the APIs for managing the Fishhead Labs Portal data.
//!
//! The current method is to pull directly from GraphQL (provided by the cache project). This will manage
//! calls like refreshing the cache data from Google Sheets.

use std::env;

use anyhow::Result;
use cache::*;
use std::sync::Arc;
use tokio::runtime::Runtime;
use warp::{http::Response, Filter};

// use juniper::{
//   tests::{model::Database, schema::Query},
//   EmptyMutation, RootNode,
// };
// use warp::{http::Response, Filter};

// type Schema = RootNode<'static, Query, EmptyMutation<Database>>;

// fn schema() -> Schema {
//   Schema::new(Query, EmptyMutation::<Database>::new())
// }

#[tokio::main]
async fn main() {
   env::set_var(
      "RUST_LOG",
      "debug, warp_server=info, cache=debug, client=debug, postgres=debug",
   );
   env_logger::init();
   log::info!("\n\n--->  Starting up the Fishhead Labs server\n");

   let log = warp::log("warp_server");

   let homepage = warp::path::end().map(|| {
      Response::builder()
         .header("content-type", "text/html")
         .body(format!(
            "<html><h1>juniper_warp</h1><div>visit <a href=\"/graphiql\">/graphiql</a></html>"
         ))
   });

   log::info!("Listening on 0.0.0.0:8080");
   let state = warp::any().map(move || JuniperCache::new(Arc::new(Runtime::new().unwrap())));
   let graphql_filter =
      juniper_warp::make_graphql_filter(JuniperCache::get_schema(), state.boxed());

   warp::serve(
      warp::get2()
         .and(warp::path("graphiql"))
         .and(juniper_warp::graphiql_filter("/graphql"))
         .or(homepage)
         .or(warp::path("graphql").and(graphql_filter))
         .with(log),
   )
   .run(([0, 0, 0, 0], 8080));
}

pub fn run_query(
   ctx: &JuniperCache,
   query: &str,
   variables: juniper::Variables,
) -> Result<juniper::Value> {
   println!("Running the query:\n{}\n{:#?}", query, variables);
   let (res, error) =
      juniper::execute(query, None, &JuniperCache::get_schema(), &variables, &ctx).unwrap();

   match error.len() {
      0 => log::debug!("res:\n{:#?}", res),
      _ => panic!(
         "Got an unexpected error from import_workbook query:\nerror\n{}\nquery\n{}\nvariables\n{}"
      ),
   }

   Ok(res)
}

#[test]
fn test_task_info() {
   env_logger::init();
   use cache::transforms::ImportWorkbook;
   use juniper::ToInputValue;

   // Test:
   // - Run ImportWorkbook mutation
   // - Query the task
   // - Async Importer should update the Task once a second
   // - Finish importer after 5 seconds, updating the TaskInfo status to finished
   // - Poll task every 2 seconds, assert that status is not equal
   // - Serde
   //    - Check generic data type can be deserialized into taskinfo
   //    - _t type data is included in data as "ImportWorkbookState"

   log::debug!("Testing the task info");

   let ctx = JuniperCache::new(Arc::new(Runtime::new().unwrap()));
   let import_workbook_query = r#"
   mutation Importer($input: ImportWorkbook!) {
      importWorkbook(input: $input) {
        guid
        state
        name
        status
      }
    }
   "#;

   let mut import_config = juniper::Variables::new();
   import_config.insert(
      "input".to_string(),
      ImportWorkbook {
         sheet_id: "1Y-swLK2kje-BSFIXIVNQNejNsLO8GamTST9LmZsPwLI".to_string(),
         sheet_names: None,
      }
      .to_input_value(),
   );

   let task = run_query(&ctx, import_workbook_query, import_config).unwrap();

   log::debug!("Task:\n{:#?}", task);

   let _get_task_query = r#"
    query {
       tasks
    }
   "#;

   // let reload_query = r#"
   //   mutation ImportData {
   //     importData(sheetId: "1t-NhBJF5n4lmO2ZQugu0MMvAdaCSwaUNxKTHwNORCoE") {isSuccess message}
   //   }
   // "#;
}
