module Widget = {
  module Control = {
    type t =
      | Remove
      | Maximize
      | Refresh
      | Configure;
  };

  type t = {controls: list(Control.t)};

  module ThreeColumn = {
    [@react.component]
    let make = (~children) => {
      <div className="ThreeColumn"> "Placeholder for ThreeColumn"->ReasonReact.string <br /> children </div>;
    };
  };

  module FourColumn = {
    [@react.component]
    let make = (~children) => {
      <div className="ThreeColumn"> "Placeholder for FourColumn"->ReasonReact.string <br /> children </div>;
    };
  };

  module EightColumn = {
    [@react.component]
    let make = (~children) => {
      <div className="ThreeColumn"> "Placeholder for EightColumn"->ReasonReact.string <br /> children </div>;
    };
  };

  module Full = {
    [@react.component]
    let make = (~children) => {
      <div className="ThreeColumn">
        "Placeholder for TwelveColumn"->ReasonReact.string
        <br />
        children
      </div>;
    };
  };
};

module DatabaseWidget = {
  [@react.component]
  let make = () => {
    <div className="DatabaseWidget"> "Placeholder for DatabaseWidget"->ReasonReact.string </div>;
  };
};

module TaskWidget = {
  [@react.component]
  let make = () => {
    <div className="TaskWidget"> "Placeholder for TaskWidget"->ReasonReact.string </div>;
  };
};

[@react.component]
let make = () => {
  <div className="Dashboard"> "Placeholder for Dashboard - React"->ReasonReact.string </div>;
};
