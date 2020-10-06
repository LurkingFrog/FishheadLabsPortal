module Widget = {
  module Control = {
    type t =
      | Remove
      | Maximize
      | Refresh
      | Configure;

    [@react.component]
    let make = (~props, ~children) => React.cloneElement(children, props);
  };

  let buildControls = controls =>
    controls
    |> Array.map((control: Control.t) =>
         switch (control) {
         | Remove =>
           <Control
             props={"data-toggle": "tooltip", "data-placement": "top", "title": "Remove Widget"}
             key={Uuid.make()}>
             <li>
               <div href="#" className="widget-control-right widget-remove anchor">
                 <span className="fa fa-times" />
               </div>
             </li>
           </Control>
         | Maximize =>
           <Control
             props={"data-toggle": "tooltip", "data-placement": "top", "title": "Full Screen"}
             key={Uuid.make()}>
             <li>
               <div href="#" className="widget-control-right widget-remove anchor">
                 <span className="fa fa-expand" />
               </div>
             </li>
           </Control>
         | Refresh =>
           <Control
             props={"data-toggle": "tooltip", "data-placement": "top", "title": "Refresh Widget"}
             key={Uuid.make()}>
             <li>
               <div href="#" className="widget-control-right widget-remove anchor">
                 <span className="fa fa-refresh" />
               </div>
             </li>
           </Control>
         | Configure =>
           <Control
             props={"data-toggle": "tooltip", "data-placement": "top", "title": "Configure Widget"}
             key={Uuid.make()}>
             <li>
               <div href="#" className="widget-control-right widget-remove anchor">
                 <span className="fa fa-cog" />
               </div>
             </li>
           </Control>
         }
       )
    |> ReasonReact.array;

  type t = {
    width: int,
    height: int,
    page: option(string),
    controls: array(Control.t),
  };

  module ThreeColumn = {
    [@react.component]
    let make = (~children) => {
      <div className="col-md-3"> children </div>;
    };
  };

  module FourColumn = {
    [@react.component]
    let make = (~children) => {
      <div className="col-md-4"> children </div>;
    };
  };

  module EightColumn = {
    [@react.component]
    let make = (~children) => {
      <div className="col-md-8"> children </div>;
    };
  };

  module Full = {
    [@react.component]
    let make = (~children) => {
      <div className="block-full-width"> children </div>;
    };
  };

  [@react.component]
  let make = (~definition, ~children) => {
    let controls = <div className="widget-controls"> {definition.controls |> buildControls} </div>;

    let dispatch = Cache.useDispatch();
    let onClick = _evt =>
      switch (definition.page) {
      | Some(pageUrl) => dispatch(Cache.Navigation(ChangePage(pageUrl)))
      | None => ()
      };

    let content = <div className="widget widget-default widget-item-icon" onClick> children controls </div>;
    switch (definition.width) {
    | 3 => <ThreeColumn> content </ThreeColumn>
    | 4 => <FourColumn> content </FourColumn>
    | 8 => <EightColumn> content </EightColumn>
    | 12 => <Full> content </Full>
    | x => Invalid_argument(Printf.sprintf("Cannot make a widget of width %i", x)) |> raise
    };
  };
};

module DatabaseWidget = {
  let definition: Widget.t = {width: 3, height: 1, controls: [|Remove|], page: Some("/database")};

  [@react.component]
  let make = () => {
    <Widget definition>
      <div className="widget-item-left"> <span className="fa fa-envelope" /> </div>
      <div className="widget-data">
        <div className="widget-int num-count"> "Database Stuff"->ReasonReact.string </div>
        <div className="widget-title"> "New messages"->ReasonReact.string </div>
        <div className="widget-subtitle"> "In your mailbox"->ReasonReact.string </div>
      </div>
    </Widget>;
  };
};

module TaskWidget = {
  let definition: Widget.t = {width: 3, height: 1, controls: [|Remove|], page: Some("/tasks")};

  [@react.component]
  let make = () => {
    <Widget definition>
      <div className="widget-item-left"> <span className="fa fa-envelope" /> </div>
      <div className="widget-data">
        <div className="widget-int num-count"> "Task Stuff"->ReasonReact.string </div>
        <div className="widget-title"> "New messages"->ReasonReact.string </div>
        <div className="widget-subtitle"> "In your mailbox"->ReasonReact.string </div>
      </div>
    </Widget>;
  };
};

let buildWidgets = x =>
  x
  |> Array.fold_left(
       ((currentWidth, widgets), conf: Configuration.Widget.t) => {
         let (definition, widget) =
           switch (conf.widgetType) {
           | Database => (DatabaseWidget.definition, <DatabaseWidget key={conf.key} />)
           | Task => (TaskWidget.definition, <TaskWidget key={conf.key} />)
           };

         let width = currentWidth + definition.width;

         (
           width / 12,
           width > 12
             ? Array.append(widgets, [|[|widget|]|])
             : {
               let index = Array.length(widgets) - 1;
               let last = widgets[index] |> Array.append([|widget|]);
               widgets[index] = last;
               widgets;
             },
         );
       },
       (0, [|[||]|]),
     )
  |> (
    ((_, widgets)) =>
      widgets
      |> Array.map(row => <div className="row" key={Uuid.make()}> {row |> ReasonReact.array} </div>)
      |> ReasonReact.array
  );

[@react.component]
let make = () => {
  let conf = Cache.useSelector(Cache.Selectors.configuration);
  let widgets = conf.dashboard.widgets |> buildWidgets;
  <div className="Dashboard"> widgets </div>;
};
