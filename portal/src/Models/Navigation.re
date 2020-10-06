/** Handle synching the navigational elements
  *
  */;

type t = {
  currentPage: string,
  showSidebar: bool,
};

let default = () => {currentPage: "Dashboard", showSidebar: true};

type action =
  | ChangePage(string);

let reducer = (state: t, action) => {
  switch (action) {
  | ChangePage(newPath) =>
    ReasonReactRouter.push(newPath)->ignore;
    {...state, currentPage: newPath};
  };
};
