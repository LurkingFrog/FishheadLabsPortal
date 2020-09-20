/* The data structure/state of the software

   This is the "global" data repository of the portal.

   TODO:
   Add on change event emitters

   */

// Session specific data
//
// This manages items that control the session
module Local = {
  type t = {session: option(Session.t)};

  let default = () => {session: None};
};

// Items that are synced with a remote server
//
//  This will eventually be replaced with the project Tyrell's WASM to automatically sync with other nodes
module Shared = {
  type t = {organizations: Belt.HashMap.String.t(Organization.t)};
  let default = () => {organizations: Belt.HashMap.String.make(~hintSize=10)};
};

// The full state of the system
type t = {
  local: Local.t,
  shared: Shared.t,
};

let default = () => {local: Local.default(), shared: Shared.default()};

/*
 Error: Unbound module Session
 Hint: Did you mean Session?
 */
