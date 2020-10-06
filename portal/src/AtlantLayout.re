module Logo = {
  [@react.component]
  let make = (~id) => {
    <li className="xn-logo" key=id id>
      <div className="anchor" href="index.html"> "ATLANT"->ReasonReact.string </div>
      <div onClick={_evt => Js.log("#")} className="x-navigation-control" />
    </li>;
  };
};

module Profile = {
  [@react.component]
  let make = (~id) => {
    <li className="xn-profile" key=id id>
      <div onClick={_evt => Js.log("#")} className="profile-mini">
        <img src="assets/images/users/avatar.jpg" alt="John Doe" />
      </div>
      <div className="profile">
        <div className="profile-image"> <img src="assets/images/users/avatar.jpg" alt="John Doe" /> </div>
        <div className="profile-data">
          <div className="profile-data-name"> "John Doe"->ReasonReact.string </div>
          <div className="profile-data-title"> "Web Developer/Designer"->ReasonReact.string </div>
        </div>
        <div className="profile-controls">
          <div href="pages-profile.html" className="anchor profile-control-left">
            <span className="fa fa-info" />
          </div>
          <div href="pages-messages.html" className="anchor profile-control-right">
            <span className="fa fa-envelope" />
          </div>
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
        <div className="anchor" onClick={onClick(key, config.link)} id=key>
          icon
          <span className="xn-text"> config.name->ReasonReact.string </span>
        </div>
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
      newConfig("Activity", ~icon="fa-tasks", ~link="/activity"),
    |];
    let menuItems =
      Array.map(config => <MenuItem config prefix="SideBar" key={Uuid.make()} />, menuConfig)
      ->ReasonReact.array;

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

module HorizontalNavbar = {
  let toggleSidebar = _event => {
    Js.log("Clicked the toggle sidebar");
  };

  [@react.component]
  let make = () => {
    <ul className="x-navigation x-navigation-horizontal x-navigation-panel">
      <li className="xn-icon-button">
        <div onClick={_evt => Js.log("#")} className="anchor x-navigation-minimize">
          <span className="fa fa-dedent" />
        </div>
      </li>
      <li className="xn-search">
        <form role="form"> <input type_="text" name="search" placeholder="Search..." /> </form>
      </li>
      <li className="xn-icon-button pull-right last">
        <div className="anchor" href="#"> <span className="fa fa-power-off" /> </div>
        <ul className="xn-drop-left animated zoomIn">
          <li>
            <div className="anchor" href="pages-lock-screen.html">
              <span className="fa fa-lock" />
              "Lock Screen"->ReasonReact.string
            </div>
          </li>
          <li>
            <div className="mb-control" onClick={_evt => Js.log("Popup signout")}>
              <span className="fa fa-sign-out" />
              "Sign Out"->ReasonReact.string
            </div>
          </li>
        </ul>
      </li>
      <li className="xn-icon-button pull-right">
        <div className="anchor" href="#"> <span className="fa fa-comments" /> </div>
        <div className="informer informer-danger"> "4"->ReasonReact.string </div>
        <div className="panel panel-primary animated zoomIn xn-drop-left xn-panel-dragging">
          <div className="panel-heading">
            <h3 className="panel-title">
              <span className="fa fa-comments" />
              "Messages"->ReasonReact.string
            </h3>
            <div className="pull-right">
              <span className="label label-danger"> "4 new"->ReasonReact.string </span>
            </div>
          </div>
          <div className="panel-body list-group list-group-contacts scroll">
            /*style="height: 200px">*/

              <div onClick={_evt => Js.log("#")} className="anchor list-group-item">
                <div className="list-group-status status-online" />
                <img src="assets/images/users/user2.jpg" className="pull-left" alt="John Doe" />
                <span className="contacts-title"> "John Doe"->ReasonReact.string </span>
                <p> "Praesent placerat tellus id augue condimentum"->ReasonReact.string </p>
              </div>
              <div onClick={_evt => Js.log("#")} className="anchor list-group-item">
                <div className="list-group-status status-away" />
                <img src="assets/images/users/user.jpg" className="pull-left" alt="Dmitry Ivaniuk" />
                <span className="contacts-title"> "Dmitry Ivaniuk"->ReasonReact.string </span>
                <p> "Donec risus sapien, sagittis et magna quis"->ReasonReact.string </p>
              </div>
              <div onClick={_evt => Js.log("#")} className="anchor list-group-item">
                <div className="list-group-status status-away" />
                <img src="assets/images/users/user3.jpg" className="pull-left" alt="Nadia Ali" />
                <span className="contacts-title"> "Nadia Ali"->ReasonReact.string </span>
                <p> "Mauris vel eros ut nunc rhoncus cursus sed"->ReasonReact.string </p>
              </div>
              <div onClick={_evt => Js.log("#")} className="anchor list-group-item">
                <div className="list-group-status status-offline" />
                <img src="assets/images/users/user6.jpg" className="pull-left" alt="Darth Vader" />
                <span className="contacts-title"> "Darth Vader"->ReasonReact.string </span>
                <p> "I want my money back!"->ReasonReact.string </p>
              </div>
            </div>
          <div className="panel-footer text-center">
            <div className="anchor" href="pages-messages.html">
              "Show all messages"->ReasonReact.string
            </div>
          </div>
        </div>
      </li>
      <li className="xn-icon-button pull-right">
        <div className="anchor" href="#"> <span className="fa fa-tasks" /> </div>
        <div className="informer informer-warning"> "3"->ReasonReact.string </div>
        <div className="panel panel-primary animated zoomIn xn-drop-left xn-panel-dragging">
          <div className="panel-heading">
            <h3 className="panel-title"> <span className="fa fa-tasks" /> "Tasks"->ReasonReact.string </h3>
            <div className="pull-right">
              <span className="label label-warning"> "3 active"->ReasonReact.string </span>
            </div>
          </div>
          <div className="panel-body list-group scroll" style={ReactDOMRe.Style.make(~height="200px", ())}>
            <div className="anchor list-group-item" href="#">
              <strong> "Phasellus augue arcu, elementum"->ReasonReact.string </strong>
              <div className="progress progress-small progress-striped active">
                <div
                  className="progress-bar progress-bar-danger"
                  role="progressbar"
                  ariaValuenow=50.0
                  ariaValuemin=0.0
                  ariaValuemax=100.0
                  style={ReactDOMRe.Style.make(~width="50%", ())} /* style="width: 50%;"*/>
                  "50%"->ReasonReact.string
                </div>
              </div>
              <small className="text-muted"> "John Doe, 25 Sep 2017 / 50%"->ReasonReact.string </small>
            </div>
            <div className="anchor list-group-item" href="#">
              <strong> "Aenean ac cursus"->ReasonReact.string </strong>
              <div className="progress progress-small progress-striped active">
                <div
                  className="progress-bar progress-bar-warning"
                  role="progressbar"
                  ariaValuenow=80.0
                  ariaValuemin=0.0
                  ariaValuemax=100.0
                  style={ReactDOMRe.Style.make(~width="80%", ())}>
                  "80%"->ReasonReact.string
                </div>
              </div>
              <small className="text-muted"> "Dmitry Ivaniuk, 24 Sep 2017 / 80%"->ReasonReact.string </small>
            </div>
            <div className="anchor list-group-item" href="#">
              <strong> "Lorem ipsum dolor"->ReasonReact.string </strong>
              <div className="progress progress-small progress-striped active">
                <div
                  className="progress-bar progress-bar-success"
                  role="progressbar"
                  ariaValuenow=95.0
                  ariaValuemin=0.0
                  ariaValuemax=100.0
                  style={ReactDOMRe.Style.make(~width="95%", ())}>
                  "95%"->ReasonReact.string
                </div>
              </div>
              <small className="text-muted"> "John Doe, 23 Sep 2017 / 95%"->ReasonReact.string </small>
            </div>
            <div className="anchor list-group-item" href="#">
              <strong> "Cras suscipit ac quam at tincidunt."->ReasonReact.string </strong>
              <div className="progress progress-small">
                <div
                  className="progress-bar"
                  role="progressbar"
                  ariaValuenow=100.0
                  ariaValuemin=0.0
                  ariaValuemax=100.0
                  style={ReactDOMRe.Style.make(~width="100%", ())}>
                  "100%"->ReasonReact.string
                </div>
              </div>
              <small className="text-muted"> "John Doe, 21 Sep 2017"->ReasonReact.string </small>
              <small className="text-success"> "Done"->ReasonReact.string </small>
            </div>
          </div>
          <div className="panel-footer text-center">
            <div className="anchor" href="pages-tasks.html"> "Show all tasks"->ReasonReact.string </div>
          </div>
        </div>
      </li>
      <li className="xn-icon-button pull-right">
        <div className="anchor" href="#"> <span className="flag flag-gb" /> </div>
        <ul className="xn-drop-left xn-drop-white animated zoomIn">
          <li>
            <div className="anchor" href="#">
              <span className="flag flag-gb" />
              "English"->ReasonReact.string
            </div>
          </li>
          <li>
            <div className="anchor" href="#">
              <span className="flag flag-de" />
              "Deutsch"->ReasonReact.string
            </div>
          </li>
          <li>
            <div className="anchor" href="#">
              <span className="flag flag-cn" />
              "Chinese"->ReasonReact.string
            </div>
          </li>
        </ul>
      </li>
    </ul>;
  };
};

module Breadcrumbs = {
  [@react.component]
  let make = () => {
    let navigation = Cache.useSelector(Cache.Selectors.navigation);
    <ul className="breadcrumb">
      <li> <div className="anchor"> "Home"->ReasonReact.string </div> </li>
      <li className="active"> navigation.currentPage->ReasonReact.string </li>
    </ul>;
  };
};

module PageTitle = {
  [@react.component]
  let make = () => {
    <div className="PageTitle"> "Placeholder for PageTitle"->ReasonReact.string </div>;
  };
};

[@react.component]
let make = (~children) => {
  let navigation = Cache.useSelector(Cache.Selectors.navigation);
  let _containerClass = navigation.showSidebar ? "page-container" : "page-navigation-toggled";

  <div className="page-container">
    <SideBar />
    <div className="page-content">
      <HorizontalNavbar />
      <Breadcrumbs />
      // <PageTitle />
      <div className="page-content-wrap"> children </div>
    </div>
  </div>;
};
