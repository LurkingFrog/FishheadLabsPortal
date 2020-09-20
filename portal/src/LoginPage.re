module Login = {
  [@react.component]
  let make = (~dispatch, ~state: Cache.t) => {
    Js.log("Running the Login Page");
    Actions.Session.(
      <div>
        {React.string("Click to login")}
        <div> <button onClick={_ => dispatch(Login)}> {React.string("Login")} </button> </div>
      </div>
    );
  };
};

[@react.component]
let make = (~state: Cache.t, ~dispatch) => {
  let body =
    switch (state.local.session |> Session.isLoggedIn) {
    | false => <Login state dispatch />
    | true => React.string("Already logged in")
    };
  <div> body </div>;
};
