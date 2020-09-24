/* The data structure/state of the software

   This is the "global" data repository of the portal.

   TODO:
   Add on change event emitters

   */

// Local Browser specific data
module Local = {
  type t = {session: option(Session.t)};

  let default = () => {session: None};
};

// Items that are synced with a remote server
//
// This will eventually be replaced with the project Tyrell's WASM to automatically sync with other nodes,
// as well as hook in on change events.
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

module Selectors = {
  let state = state => state;
  let session = state => state.local.session;
};

let reducer = (state: t, action) => {
  Js.log("In the reducer with action");
  Js.log(action);
  switch (action) {
  | Actions.Session(Create(session)) => {
      ...state,
      local: {
        session: Some(session),
      },
    }
  | Actions.Session(End) => {
      ...state,
      local: {
        session: None,
      },
    }
  };
};

// Turns this into a Reductive provider store
include ReductiveContext.Make({
  type action = Actions.t;
  type state = t;
});

let logger = (store, next) => Middleware.logger(store) @@ next;

let store = Reductive.Store.create(~reducer, ~preloadedState=default(), ~enhancer=logger, ());
