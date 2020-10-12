/* The data structure/state of the software

   This is the "global" data repository of the portal.

   TODO:
   Add on change event emitters

   */

// Local Browser specific data
module Local = {
  type t = {
    session: option(Session.t),
    navigation: Navigation.t,
    // alerts: Alert.t,
  };

  let default = () => {session: None, navigation: Navigation.default()};
};

// Items that are synced with a remote server
//
// This will eventually be replaced with the project Tyrell's WASM to automatically sync with other nodes,
// as well as hook in on change events.
module Shared = {
  type t = {
    configuration: Configuration.t,
    organizations: Belt.HashMap.String.t(Organization.t),
  };
  let default = () => {
    configuration: Configuration.default(),
    organizations: Belt.HashMap.String.make(~hintSize=10),
  };
};

// The full state of the system
type t = {
  local: Local.t,
  shared: Shared.t,
};

let default = () => {local: Local.default(), shared: Shared.default()};

module Selectors = {
  let state = state => state;
  let configuration = state => state.shared.configuration;
  let session = state => state.local.session;
  let navigation = state => state.local.navigation;
};

type actions =
  | Session(Session.action)
  | Navigation(Navigation.action);

/** TODO: Add a root level Result to catch and alert the user to any errors from trying to run the reducer */
let reducer = (state: t, action) => {
  switch (action) {
  | Session(act) => {
      ...state,
      local: {
        ...state.local,
        session: Session.reducer(state.local.session, act),
      },
    }
  | Navigation(act) => {
      ...state,
      local: {
        ...state.local,
        navigation: Navigation.reducer(state.local.navigation, act),
      },
    }
  };
};

// Turns this into a Reductive provider store
include ReductiveContext.Make({
  type action = actions;
  type state = t;
});

let logger = (store, next) => Middleware.logger(store) @@ next;

let store = Reductive.Store.create(~reducer, ~preloadedState=default(), ~enhancer=logger, ());
