module Widget = {
  [@react.component]
  let make = () => {
    <div className="Widget"> "Placeholder for Database Widget"->ReasonReact.string </div>;
  };
};

module Status = {
  [@react.component]
  let make = () => {
    <div className="Status"> "Placeholder for Status"->ReasonReact.string </div>;
  };
};

module Configuration = {
  [@react.component]
  let make = () => {
    <div className="Configuration"> "Placeholder for Configuration"->ReasonReact.string </div>;
  };
};

module ImportGoogleSheet = {
  [@react.component]
  let make = () => {
    <OneColumn />;
  };
};

module ExportGoogleSheet = {
  [@react.component]
  let make = () => {
    <div className="ExportGoogleSheet"> "Placeholder for ExportGoogleSheet"->ReasonReact.string </div>;
  };
};

[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();
  Js.log(url.path);
  switch (url.path) {
  | [_, "configuration"] => <Configuration />
  | [_, "export"] => <ExportGoogleSheet />
  | [_, "import"] => <ImportGoogleSheet />
  | [_, "status"]
  | [_] => <Status />
  | _ => <PageNotFound />
  };
};
