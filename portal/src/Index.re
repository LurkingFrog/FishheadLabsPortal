/* The entrypoint to the portal */
open Webapi.Dom;

let unwrap = (value: option('a)): 'a => Belt.Option.getExn(value);

let body = document->Document.asHtmlDocument->unwrap->HtmlDocument.body->unwrap;
let head = document->Document.asHtmlDocument->unwrap->HtmlDocument.head;

let makeContainer = text => {
  let container = Document.createElement("div", document);
  container->Element.setClassName("container");
  container->Element.setId(text);

  let () = container->Element.appendChild(body);
  container;
};

// FIXME: This assumes that the full path is included, may not work for prod mode
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
    Js.log("Rendering App");
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
        | "dashboard" => <div> "The Dashboard"->ReasonReact.string </div>
        | path => <div> {("404 - Page not found. " ++ path)->ReasonReact.string} </div>
        };
      <AtlantLayout> body </AtlantLayout>;
    };
  };
};

module Root = {
  [@react.component]
  let make = () => {
    let () = linkCss("./src/index.css");
    let () = linkCss("./src/assets/css/atlant-theme-default.css");

    <div> <Cache.Provider store=Cache.store> <App /> </Cache.Provider> </div>;
  };
};

ReactDOMRe.render(<Root />, makeContainer("root"));
