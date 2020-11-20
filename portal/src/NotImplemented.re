[@react.component]
let make = (~msg=?) => {
  let url = ReasonReactRouter.useUrl();
  let msg =
    msg->Belt.Option.getWithDefault(
      "Path '" ++ List.fold_left((acc, x) => acc ++ "/" ++ x, "", url.path) ++ "'",
    )
    |> ReasonReact.string;
  <div className="row">
    <div className="col-md-12">
      <div className="error-container">
        <div className="error-code"> "404"->ReasonReact.string </div>
        <div className="error-text"> "Not Implemented"->ReasonReact.string </div>
        <div className="error-subtext">
          "This feature/page hasn't been implemented yet. When done, it is expected to do the following:"
          ->ReasonReact.string
          <br />
          msg
        </div>
        <div className="error-actions">
          <div className="row">
            <div className="col-md-6">
              <button
                className="btn btn-info btn-block btn-lg"
                onClick={_evt => ReasonReactRouter.push("/dashboard")}>
                "Back to dashboard"->ReasonReact.string
              </button>
            </div>
            <div className="col-md-6">
              <button className="btn btn-primary btn-block btn-lg" onClick={_evt => Helpers.back()}>
                "Previous page"->ReasonReact.string
              </button>
            </div>
          </div>
        </div>
        <div className="error-subtext">
          "Or you can use search to find anything you need."->ReasonReact.string
        </div>
        <div className="row">
          <div className="col-md-12">
            <div className="input-group">
              <input type_="text" placeholder="Search..." className="form-control" />
              <div className="input-group-btn">
                <button className="btn btn-primary"> <span className="fa fa-search" /> </button>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>;
};
