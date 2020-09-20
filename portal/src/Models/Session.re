// Session specific data
//
// This manages items that control session authentication
type t = {
  currentUser: option(string),
  token: option(string),
};

let default = () => {currentUser: None, token: None};

let isLoggedIn = (session: option(t)) => {
  switch (session) {
  | None => false
  | Some(_session) => true
  };
};
