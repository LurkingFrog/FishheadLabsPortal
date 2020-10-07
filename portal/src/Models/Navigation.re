/** Handle synching the navigational elements
  *
  */;

module MenuItem = {
  type t = {
    name: string,
    key: string,
    className: string,
    iconClass: option(string),
    parent: option(string),
    children: array(t),
    uri: option(string),
    toolTip: option(string),
    isOpen: bool,
    isVisible: bool,
  };

  let newMenuItem =
      (
        ~key=?,
        ~className="",
        ~iconClass=?,
        ~parent=?,
        ~children=[||],
        ~uri=?,
        ~toolTip=?,
        ~isOpen=false,
        ~isVisible=true,
        name,
      ) => {
    let key = Belt.Option.getWithDefault(key, Uuid.make());
    {
      key,
      name,
      className,
      iconClass,
      parent,
      children: children |> Array.map(child => {...child, parent: Some(key)}),
      uri,
      toolTip,
      isOpen,
      isVisible,
    };
  };
};

type link = {
  uri: string,
  display: option(string),
};

type t = {
  currentPage: string,
  showSidebar: bool,
  menuItems: array(MenuItem.t),
};

let default = () => {
  MenuItem.{
    currentPage: "Dashboard",
    showSidebar: true,
    menuItems: [|
      newMenuItem("Dashboard", ~iconClass="fa-dashboard", ~uri="/dashboard"),
      newMenuItem(
        "Database",
        ~iconClass="fa-database",
        ~children=[|
          newMenuItem("Import", ~iconClass="fa-import", ~uri="/database/import"),
          newMenuItem("Export", ~iconClass="fa-export", ~uri="/database/export"),
        |],
        ~uri="/database",
      ),
      newMenuItem("Activity", ~iconClass="fa-tasks", ~uri="/activity"),
    |],
  };
};

type action =
  | ChangePage(string)
  | ToggleSidebar
  | MenuItemClick(string);

let reducer = (state: t, action) => {
  switch (action) {
  | ChangePage(newPath) =>
    ReasonReactRouter.push(newPath)->ignore;
    // TODO: Validate that the URI syncs with the active menu item
    {...state, currentPage: newPath};
  | ToggleSidebar => {...state, showSidebar: state.showSidebar ? false : true}
  | MenuItemClick(key) =>
    // Loop through each of the root items
    {
      ...state,
      menuItems:
        state.menuItems
        |> Array.map((item: MenuItem.t) => {
             Js.log(item);
             key == item.key
               ? {
                 Js.log("Matched the item: " ++ key);
                 {...item, isOpen: item.isOpen ? false : true};
               }
               : item;
             // If it is the clicked item:
             // Set page to active
             // Change page if link
           }),
    }
  };
};
