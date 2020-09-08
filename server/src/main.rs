//! The primary warp data server
//!
//! This is the backend combining the APIs for managing the Fishhead Labs Portal data.
//!
//! The current method is to pull directly from GraphQL (provided by the cache project). This will manage
//! calls like refreshing the cache data from Google Sheets.

use std::env;

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
