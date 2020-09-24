// Session specific data
//
// This manages items that control session authentication
type t = {
  currentUser: string,
  token: string,
};

let isLoggedIn = (session: option(t)) => {
  Js.log("Checking auth");
  switch (session) {
  | None => false
  | Some(_session) => true
  };
};
