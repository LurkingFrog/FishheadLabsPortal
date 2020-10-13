[@react.component]
let make = () => {
  <div className="row">
    <div className="col-md-12">
      <form className="form-horizontal">
        <div className="panel panel-default">
          <div className="panel-heading">
            <h3 className="panel-title">
              <strong> "One Column</strong> Layout"->ReasonReact.string </strong>
            </h3>
            <ul className="panel-controls">
              <li> <a href="#" className="panel-remove"> <span className="fa fa-times" /> </a> </li>
            </ul>
          </div>
          <div className="panel-body">
            <p>
              "This is non libero bibendum, scelerisque arcu id, placerat nunc. Integer ullamcorper rutrum dui
                      eget porta. Fusce enim dui, pulvinar a augue nec, dapibus hendrerit mauris. Praesent efficitur,
                      elit non convallis faucibus, enim sapien suscipit mi, sit amet fringilla felis arcu id sem.
                      Phasellus semper felis in odio convallis, et venenatis nisl posuere. Morbi non aliquet magna, a
                      consectetur risus. Vivamus quis tellus eros. Nulla sagittis nisi sit amet orci consectetur
                      laoreet. Vivamus volutpat erat ac vulputate laoreet. Phasellus eu ipsum massa."
              ->ReasonReact.string
            </p>
          </div>
          <div className="panel-body">
            <div className="form-group">
              <label className="col-md-3 col-xs-12 control-label"> "Text Field"->ReasonReact.string </label>
              <div className="col-md-6 col-xs-12">
                <div className="input-group">
                  <span className="input-group-addon"> <span className="fa fa-pencil" /> </span>
                  <input type_="text" className="form-control" />
                </div>
                <span className="help-block"> "This is sample of text field"->ReasonReact.string </span>
              </div>
            </div>
            <div className="form-group">
              <label className="col-md-3 col-xs-12 control-label"> "Password"->ReasonReact.string </label>
              <div className="col-md-6 col-xs-12">
                <div className="input-group">
                  <span className="input-group-addon"> <span className="fa fa-unlock-alt" /> </span>
                  <input type_="password" className="form-control" />
                </div>
                <span className="help-block"> "Password field sample"->ReasonReact.string </span>
              </div>
            </div>
            <div className="form-group">
              <label className="col-md-3 col-xs-12 control-label"> "Datepicker"->ReasonReact.string </label>
              <div className="col-md-6 col-xs-12">
                <div className="input-group">
                  <span className="input-group-addon"> <span className="fa fa-calendar" /> </span>
                  <input type_="text" className="form-control datepicker" value="2017-11-01" />
                </div>
                <span className="help-block">
                  "Click on input field to get datepicker"->ReasonReact.string
                </span>
              </div>
            </div>
            <div className="form-group">
              <label className="col-md-3 col-xs-12 control-label"> "Textarea"->ReasonReact.string </label>
              <div className="col-md-6 col-xs-12">
                <textarea className="form-control" rows=5 />
                <span className="help-block"> "Default textarea field"->ReasonReact.string </span>
              </div>
            </div>
            <div className="form-group">
              <label className="col-md-3 col-xs-12 control-label"> "Tags"->ReasonReact.string </label>
              <div className="col-md-6 col-xs-12">
                <input type_="text" className="tagsinput" value="First,Second,Third" />
                <span className="help-block"> "Default textarea field"->ReasonReact.string </span>
              </div>
            </div>
            <div className="form-group">
              <label className="col-md-3 col-xs-12 control-label"> "Select"->ReasonReact.string </label>
              <div className="col-md-6 col-xs-12">
                <select className="form-control select">
                  <option> "Option 1"->ReasonReact.string </option>
                  <option> "Option 2"->ReasonReact.string </option>
                  <option> "Option 3"->ReasonReact.string </option>
                  <option> "Option 4"->ReasonReact.string </option>
                  <option> "Option 5"->ReasonReact.string </option>
                </select>
                <span className="help-block"> "Select box example"->ReasonReact.string </span>
              </div>
            </div>
            <div className="form-group">
              <label className="col-md-3 col-xs-12 control-label"> "File"->ReasonReact.string </label>
              <div className="col-md-6 col-xs-12">
                <input
                  type_="file"
                  className="fileinput btn-primary"
                  name="filename"
                  id="filename"
                  title="Browse file"
                />
                <span className="help-block"> "Input type file"->ReasonReact.string </span>
              </div>
            </div>
            <div className="form-group">
              <label className="col-md-3 col-xs-12 control-label"> "Checkbox"->ReasonReact.string </label>
              <div className="col-md-6 col-xs-12">
                <label className="check">
                  <input type_="checkbox" className="icheckbox" checked=true />
                  "Checkbox title"->ReasonReact.string
                </label>
                <span className="help-block"> "Checkbox sample, easy to use"->ReasonReact.string </span>
              </div>
            </div>
          </div>
          <div className="panel-footer">
            <button className="btn btn-default"> "Clear Form"->ReasonReact.string </button>
            <button className="btn btn-primary pull-right"> "Submit"->ReasonReact.string </button>
          </div>
        </div>
      </form>
    </div>
  </div>;
};
