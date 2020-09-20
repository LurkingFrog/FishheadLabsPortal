// Entry point

// ReactDOMRe.render(<Layout.App />, root);

// // We're using raw DOM manipulations here, to avoid making you read
// // ReasonReact when you might precisely be trying to learn it for the first
// // time through the examples later.
// let style = document##createElement("style");
// document##head##appendChild(style);
// style##innerHTML #= ExampleStyles.style;

// // // All 4 examples.
// ReactDOMRe.render(
//   <BlinkingGreeting> {React.string("Hello!")} </BlinkingGreeting>,
//   makeContainer("Blinking Greeting"),
// );

// ReactDOMRe.render(<ReducerFromReactJSDocs />, makeContainer("Reducer From ReactJS Docs"));

// ReactDOMRe.render(<FetchedDogPictures />, makeContainer("Fetched Dog Pictures"));

// ReactDOMRe.render(<ReasonUsingJSUsingReason />, makeContainer("Trying chane ithane again"));

let reducer = (state: Cache.t, action) => {
  Js.log("In the reducer with action");
  Js.log(action);
  switch (action) {
  | Actions.Session.Login => {
      ...state,
      local: {
        session: Some(Session.default()),
      },
    }
  | Actions.Session.Logout => {
      ...state,
      local: {
        session: None,
      },
    }
  };
};

module App = {
  [@react.component]
  let make = () => {
    let (state, dispatch) = React.useReducer(reducer, Cache.default());

    // Check Login
    let isLoggedIn = None->Session.isLoggedIn;

    let url = ReasonReactRouter.useUrl();

    Js.log(url);

    let body =
      switch (isLoggedIn) {
      | false => <LoginPage state dispatch />
      | true => React.string("Already logged in")
      };
    <div> body </div>;
  };
};

[@bs.val] external document: Js.t({..}) = "document";

let makeContainer = text => {
  let container = document##createElement("div");
  container##className #= "container";

  let title = document##createElement("div");
  title##className #= "containerTitle";
  title##innerText #= text;

  let content = document##createElement("div");
  content##className #= "containerContent";

  let () = container##appendChild(content);
  let () = document##body##appendChild(container);

  content;
};

ReactDOMRe.render(<App />, makeContainer("root"));
