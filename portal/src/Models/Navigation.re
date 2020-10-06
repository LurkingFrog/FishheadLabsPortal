/** Handle synching the navigational elements
  *
  */;

type t = {
  currentPage: string,
  showSidebar: bool,
};

let default = () => {currentPage: "", showSidebar: true};

type action =
  | ChangePage(string);

let reducer = (state: t, action) => {
  switch (action) {
  | ChangePage(newPage) =>
    ReasonReactRouter.push(newPage)->ignore;
    {...state, currentPage: newPage};
  };
};
