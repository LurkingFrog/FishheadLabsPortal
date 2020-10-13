module Widget = {
  [@react.component]
  let make = () => {
    <div className="Widget"> "Placeholder for Widget"->ReasonReact.string </div>;
  };
};

module ImportGoogleSheet = {
  [@react.component]
  let make = () => {
    <div className="ImportGoogleSheet"> "Placeholder for ImportGoogleSheet"->ReasonReact.string </div>;
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
  <div className="Database"> "Placeholder for Database root page"->ReasonReact.string </div>;
};