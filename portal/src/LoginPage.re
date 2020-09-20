[@react.component]
let make = (~dispatch, ~state: Cache.t) => {
  Js.log("Running the Login Page");
  Actions.Session.(
    <div className="login-container login-v2">
      <div className="login-box animated fadeInDown">
        <div className="login-body">
          <div className="login-title">
            <strong> "Welcome"->ReasonReact.string </strong>
            ", Please login."->ReasonReact.string
          </div>
          <form action="index.html" className="form-horizontal" method="post">
            <div className="form-group">
              <div className="col-md-12">
                <div className="input-group">
                  <div className="input-group-addon"> <span className="fa fa-user" /> </div>
                  <input type_="text" className="form-control" placeholder="Username" />
                </div>
              </div>
            </div>
            <div className="form-group">
              <div className="col-md-12">
                <div className="input-group">
                  <div className="input-group-addon"> <span className="fa fa-lock" /> </div>
                  <input type_="password" className="form-control" placeholder="Password" />
                </div>
              </div>
            </div>
            <div className="form-group">
              <div className="col-md-6"> <a href="#"> "Forgot your password?"->ReasonReact.string </a> </div>
              <div className="col-md-6 text-right">
                <a href="#"> "Create an account"->ReasonReact.string </a>
              </div>
            </div>
            <div className="form-group">
              <div className="col-md-12">
                <button className="btn btn-primary btn-lg btn-block" onClick={_ => dispatch(Login)}>
                  "Login"->ReasonReact.string
                </button>
              </div>
            </div>
          </form>
        </div>
        <div className="login-footer">
          <div className="pull-left"> "&copy; 2020 The Process Foundry"->ReasonReact.string </div>
          <div className="pull-right">
            <a href="#"> "About"->ReasonReact.string </a>
            " | "->ReasonReact.string
            <a href="#"> "Privacy"->ReasonReact.string </a>
            " | "->ReasonReact.string
            <a href="#"> "Contact Us"->ReasonReact.string </a>
          </div>
        </div>
      </div>
    </div>
  );
};
