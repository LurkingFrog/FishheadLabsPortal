/* The entrypoint to the portal */
[%bs.raw {|require("./assets/css/atlant-theme-default.css")|}];

open Webapi.Dom;

let unwrap = (value: option('a)): 'a => Belt.Option.getExn(value);

let body = document->Document.asHtmlDocument->unwrap->HtmlDocument.body->unwrap;
let head = document->Document.asHtmlDocument->unwrap->HtmlDocument.head;

let makeContainer = text => {
  let container = Document.createElement("div", document);
  container->Element.setClassName("RootContainer");
  container->Element.setId(text);

  let () = container->Element.appendChild(body);
  container;
};

// FIXME: This assumes that the full path is included, may not work for prod mode
// DEPRECATED: This was required because of using moduleserve.js. We can now use webpack
let linkCss = text => {
  let css = Document.createElement("link", document);
  let () = Element.setAttribute("rel", "stylesheet", css);
  let () = Element.setAttribute("type", "text/css", css);
  let () = Element.setAttribute("href", text, css);
  let () = Element.appendChild(css, head);
  ();
};

module App = {
  [@react.component]
  let make = () => {
    let _session = Cache.useSelector(Cache.Selectors.session);
    let url = ReasonReactRouter.useUrl();

    // switch (session->Session.isLoggedIn) {
    switch (true) {
    | false => <LoginPage />
    | true =>
      let body =
        switch (url.path->List.nth_opt(0)->Belt.Option.getWithDefault("")->String.lowercase_ascii) {
        | "/"
        | ""
        | "dashboard" => <Dashboard />
        | "database" => <DatabasePage />
        | path => <PageNotFound msg={"There is no page called '" ++ path ++ "'"} />
        };
      <AtlantLayout> body </AtlantLayout>;
    };
  };
};

module Root = {
  [@react.component]
  let make = () => {
    <div> <Cache.Provider store=Cache.store> <App /> </Cache.Provider> </div>;
  };
};

ReactDOMRe.render(<Root />, makeContainer("root"));
