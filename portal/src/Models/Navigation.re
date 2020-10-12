/** Handle synching the navigational elements
  *
  */;

open Errors;
open Helpers;

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
    isRoot: bool,
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
        ~isRoot=false,
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
      isRoot,
      isOpen,
      isVisible,
    };
  };

  let newRoot = () => newMenuItem(~isRoot=true, "Root");

  let addChild = (~position=None, item: t, child: t) => {
    // No duplicates allowed
    Belt.Array.getBy(item.children, x => x.key == child.key)
    |> mapSome(
         ~msg=Printf.sprintf("MenuItem key '%s' alread has a child '%s'", item.key, child.key),
         DuplicateKey,
       )
    |> kC(() => {
         let children = {...child, parent: Some(item.key)} |> insert(~position, item.children) |> getExn;
         ok({...item, children});
       });
  };
};

/** A grouped list of nested navigation items */
module Menu = {
  type displayMode =
    | Hidden
    | Minimized
    | Full;

  type t = {
    /** How the menu should display */
    display: displayMode,
    /** A key -> MenuItem lookup used for reducing events */
    lookup: Belt.HashMap.String.t(MenuItem.t),
    /** A root node to contain all the others */
    root: string,
  };

  let default = () => {
    let root = MenuItem.newRoot();
    {display: Full, lookup: [|(root.key, root)|] |> Belt.HashMap.String.fromArray, root: root.key};
  };

  let updateMenuItem = (menu: t, key: string, transform) => {
    // Change the copy in the lookup
    // Get the item from the lookup
    HM.find(
      ~notFound=Some(sprintf("MenuItem '%s' was clicked but doesn't exist in the lookup", key)),
      menu.lookup,
      key,
    )
    |> kC(transform)
    |> kC((item: MenuItem.t) => {
         item.isRoot
           ? ok(item)
           : HM.find(
               ~notFound=
                 Some(
                   sprintf("MenuItem '%s' was listed as a parent but doesn't exist in the lookup", key),
                 ),
               menu.lookup,
               item.parent |> Belt.Option.getExn,
             )
             |> kC((parent: MenuItem.t) => {
                  let index =
                    parent.children
                    ->Belt.Array.getIndexBy(item => item.parent |> Belt.Option.getExn == parent.key)
                    |> Belt.Option.getExn;
                  Belt.Array.set(parent.children, index, item) |> ignore;
                  Belt.HashMap.String.set(menu.lookup, parent.key, parent);
                  ok(item);
                })
       })
    |> kC((item: MenuItem.t) => {
         let lookup = menu.lookup;
         Belt.HashMap.String.set(lookup, item.key, item);
         ok({...menu, lookup});
       });
  };

  /** Add a list of item to the end of the the parent's list. If no parent is listed, add it to the root
   *
   * The main purpose of this function to be able to update the tree with events, since we have to either
   * flatten the tree into a list and calculate hierarchy each time, or
  */
  let rec addMenuItems = (menu: t) =>
    Array.fold_left((acc, item) => acc |> Eeyo.kC(addMenuItem(item)), ok(menu))

  and (
      /** Add a new item with children to the end of the the parent list. If no parent is listed, add it to the root */
      addMenuItem
      ) =
      (item: MenuItem.t, menu: t) => {
    item.children
    |> addMenuItems(menu)
    |> kC(menu =>
         switch (item.parent) {
         | Some(_) => ok((menu, item))
         | None =>
           let item = {...item, parent: Some(menu.root)};
           let menu =
             updateMenuItem(menu, menu.root, root =>
               ok({...root, children: insert(root.children, item) |> getExn})
             )
             |> getExn;
           ok((menu, item));
         }
       )
    |> kC(((menu, item: MenuItem.t)) => {
         Belt.HashMap.String.set(menu.lookup, item.key, item) |> ignore;
         ok(menu);
       });
  };
};

type link = {
  uri: string,
  display: option(string),
};

type t = {
  currentPage: string,
  sidebar: Menu.t,
};

let default = () => {
  open MenuItem;
  let sidebar =
    [|
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
    |]
    |> Menu.addMenuItems(Menu.default())
    |> getExn;
  {currentPage: "Dashboard", sidebar};
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
  | ToggleSidebar => {
      ...state,
      sidebar: {
        ...state.sidebar,
        display:
          switch (state.sidebar.display) {
          | Full => Minimized
          | Minimized => Full
          | Hidden => Hidden
          },
      },
    }
  | MenuItemClick(key) =>
    Js.log("Clicked menu item " ++ key);
    let sidebar =
      Menu.updateMenuItem(state.sidebar, key, item => ok({...item, isOpen: item.isOpen ? false : true}))
      |> getExn;
    {...state, sidebar};
  };
};
