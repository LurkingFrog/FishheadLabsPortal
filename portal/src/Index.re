/* The entrypoint to the portal */
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

// FIXME: This assumes that the full path is included, may not work for prod mode
let linkCss = text => {
  let css = document##createElement("link");
  let () = css##setAttribute("rel", "stylesheet");
  let () = css##setAttribute("type", "text/css");
  let () = css##setAttribute("href", text);
  let () = document##head##appendChild(css);
  ();
};

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
    let () = linkCss("./src/index.css");
    let () = linkCss("./src/assets/css/animate.min.css");
    let () = linkCss("./src/assets/css/atlant-theme-default.css");

    // Check Login

    let url = ReasonReactRouter.useUrl();

    Js.log(url);

    let body =
      switch (None->Session.isLoggedIn) {
      | false => <LoginPage state dispatch />
      | true => React.string("Already logged in. TODO: Add the main page")
      };
    <div> body </div>;
  };
};

ReactDOMRe.render(<App />, makeContainer("root"));
