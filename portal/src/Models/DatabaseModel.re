/** Manage the backend data configuration
 *
 * This includes backup/restore of the database and importing/exporting spreadsheets
 */;

module Postgres = {};
module Tyrell = {};

type t = {databases: Belt.HashMap.String.t(string)};
