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

module App = {
  [@react.component]
  let make = () => {
    Js.log("Rendering App");
    let session = Cache.useSelector(Cache.Selectors.session);

    switch (session->Session.isLoggedIn) {
    | false => <LoginPage />
    | true => <AtlantLayout />
    };
  };
};

module Root = {
  [@react.component]
  let make = () => {
    let () = linkCss("./src/index.css");
    let () = linkCss("./src/assets/css/atlant-theme-default.css");

    // Check Login

    let url = ReasonReactRouter.useUrl();

    Js.log(url);

    <div> <Cache.Provider store=Cache.store> <App /> </Cache.Provider> </div>;
  };
};

ReactDOMRe.render(<Root />, makeContainer("root"));
