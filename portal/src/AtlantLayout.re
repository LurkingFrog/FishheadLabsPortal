module Logo = {
  [@react.component]
  let make = (~id) => {
    <li className="xn-logo" key=id id>
      <a href="index.html"> "ATLANT"->ReasonReact.string </a>
      <a href="#" className="x-navigation-control" />
    </li>;
  };
};

module Profile = {
  [@react.component]
  let make = (~id) => {
    <li className="xn-profile" key=id id>
      <a href="#" className="profile-mini"> <img src="assets/images/users/avatar.jpg" alt="John Doe" /> </a>
      <div className="profile">
        <div className="profile-image"> <img src="assets/images/users/avatar.jpg" alt="John Doe" /> </div>
        <div className="profile-data">
          <div className="profile-data-name"> "John Doe"->ReasonReact.string </div>
          <div className="profile-data-title"> "Web Developer/Designer"->ReasonReact.string </div>
        </div>
        <div className="profile-controls">
          <a href="pages-profile.html" className="profile-control-left"> <span className="fa fa-info" /> </a>
          <a href="pages-messages.html" className="profile-control-right">
            <span className="fa fa-envelope" />
          </a>
        </div>
      </div>
    </li>;
  };
};

module SideBar = {
  let onClick = (id, link, event) => {
    event->ReactEvent.Synthetic.preventDefault;
    open Webapi.Dom;

    // Js.log(clicked##className);
    let elem = id->Document.getElementById(document)->Belt.Option.getExn;
    let toggle = DomTokenList.toggle("active", elem->Element.classList);
    // TODO: make the rest inactive
    toggle ? Js.log("Clicked on the active list") : Js.log("This one is inactive");

    // Change to the new page, if there is one
    link->Belt.Option.map(ReasonReactRouter.push)->ignore;
  };

  type itemConfig = {
    name: string,
    className: string,
    icon: option(string),
    children: array(itemConfig),
    link: option(string),
  };

  let newConfig = (~className="", ~icon=?, ~children=[||], ~link=?, name) => {
    {name, className, icon, children, link};
  };

  module MenuItem = {
    let rec buildItem = (config, prefix) => {
      let key = prefix ++ "__" ++ config.name->Js.String2.replace(" ", "_");
      let className =
        config.children->Array.length > 0 ? "xn-openable " ++ config.className : config.className;
      let icon =
        config.icon
        ->Belt.Option.mapWithDefault(ReasonReact.null, icon => <span className={"fa " ++ icon} />);
      let children =
        config.children->Array.length == 0
          ? ReasonReact.null
          : <ul> {Array.map(child => buildItem(child, key), config.children)->ReasonReact.array} </ul>;

      <li className key id=key>
        <a href="#" onClick={onClick(key, config.link)} id=key>
          icon
          <span className="xn-text"> config.name->ReasonReact.string </span>
        </a>
        children
      </li>;
    };

    [@react.component]
    let make = (~config, ~prefix="") => buildItem(config, prefix);
  };

  module Title = {
    [@react.component]
    let make = (~text, ~prefix="") => {
      let key = (prefix ++ text)->Js.String2.replace(" ", "_");
      <li className="xn-title" key id=key> text->ReasonReact.string </li>;
    };
  };

  [@react.component]
  let make = () => {
    let menuConfig = [|
      newConfig("Dashboard", ~icon="fa-dashboard", ~link="/dashboard"),
      newConfig(
        "Database",
        ~icon="fa-database",
        ~children=[|
          newConfig("Import", ~icon="fa-import", ~link="/database/import"),
          newConfig("Export", ~icon="fa-export", ~link="/database/export"),
        |],
        ~link="/database",
      ),
      newConfig("Tasks", ~icon="fa-tasks", ~link="/tasks"),
    |];
    let menuItems =
      Array.map(config => <MenuItem config prefix="SideBar" />, menuConfig)->ReasonReact.array;

    <div className="page-sidebar">
      <ul className="x-navigation">
        <Logo id="SideBar__Logo" />
        <Profile id="SideBar__Profile" />
        <Title text="Navigation" prefix="SideBar" />
        menuItems
      </ul>
    </div>;
  };
};

module VerticalNavBar = {
  [@react.component]
  let make = () => {
    <div className="VerticalNavBar"> "Placeholder for VerticalNavBar"->ReasonReact.string </div>;
  };
};

module Breadcrumb = {
  [@react.component]
  let make = () => {
    <div className="Breadcrumb"> "Placeholder for Breadcrumb"->ReasonReact.string </div>;
  };
};

module PageContent = {
  [@react.component]
  let make = () => {
    <div className="PageContent"> "Placeholder for PageContent"->ReasonReact.string </div>;
  };
};

let link = path =>
  <a key=path onClick={_ => ReasonReactRouter.push(path)}> {("Link To: " ++ path)->ReasonReact.string} </a>;

[@react.component]
let make = (~children) => {
  <div className="x-dashboard">
    <div className="page-container">
      <div className="page-content">
        <div className="page-content-wrap">
          <SideBar />
          <div> "I'm logged in"->ReasonReact.string </div>
          children
        </div>
      </div>
    </div>
  </div>;
};
