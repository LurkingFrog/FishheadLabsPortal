// Form state/validation
module LoginForm = {
  type status =
    | Empty
    | Valid
    | Dirty
    | Error
    | Sending;

  type t = {
    username: string,
    password: string,
    // Login using google Oauth2?
    token: option(string),
    error: option(string),
    status,
  };

  type fields =
    | Username
    | Password;

  let default = () => {username: "", password: "", token: None, error: None, status: Empty};

  // let onChange = (event, setter) => {};

  let onFocus = event => ReactEvent.Focus.target(event)##select();

  let submit = setState => {
    setState(formState => {{...formState, status: Sending}});
  };

  let sendAuth = _formState => {
    Ok("I'm the fake token");
  };
};

[@react.component]
let make = () => {
  // let (localState, )
  let (formState, setFormState) = React.useState(() => LoginForm.default());
  let dispatch = Cache.useDispatch();

  // Submit the validated form for authorization
  React.useEffect(() => {
    switch (formState.status) {
    | Sending =>
      switch (LoginForm.sendAuth(formState)) {
      | Ok(token) => Session(Create({currentUser: formState.username, token}))->dispatch
      | Error(err) => setFormState(state => {...state, status: Error, error: Some(err)})
      }
    | _ => ()
    };
    None;
  });

  let username =
    <div className="form-group">
      <div className="col-md-12">
        <div className="input-group">
          <div className="input-group-addon"> <span className="fa fa-user" /> </div>
          <input type_="text" className="form-control" placeholder="Username" />
        </div>
      </div>
    </div>;

  let password =
    <div className="form-group">
      <div className="col-md-12">
        <div className="input-group">
          <div className="input-group-addon"> <span className="fa fa-lock" /> </div>
          <input type_="password" className="form-control" placeholder="Password" />
        </div>
      </div>
    </div>;

  <div className="login-container login-v2">
    <div className="login-box animated fadeInDown">
      <div className="login-body">
        <div className="login-title">
          <strong> "Welcome"->ReasonReact.string </strong>
          ", Please login."->ReasonReact.string
        </div>
        <div className="form-horizontal">
          username
          password
          <div className="form-group">
            <div className="col-md-6"> <a href="#"> "Forgot your password?"->ReasonReact.string </a> </div>
            <div className="col-md-6 text-right">
              <a href="#"> "Create an account"->ReasonReact.string </a>
            </div>
          </div>
          <div className="form-group">
            <div className="col-md-12">
              <button
                className="btn btn-primary btn-lg btn-block" onClick={_ => LoginForm.submit(setFormState)}>
                "Login"->ReasonReact.string
              </button>
            </div>
          </div>
        </div>
      </div>
      <div className="login-footer">
        <div className="pull-left">
          <span className="fa fa-copyright" />
          "2020 The Process Foundry"->ReasonReact.string
        </div>
        <div className="pull-right">
          <a href="#"> "About"->ReasonReact.string </a>
          " | "->ReasonReact.string
          <a href="#"> "Privacy"->ReasonReact.string </a>
          " | "->ReasonReact.string
          <a href="#"> "Contact Us"->ReasonReact.string </a>
        </div>
      </div>
    </div>
  </div>;
};
