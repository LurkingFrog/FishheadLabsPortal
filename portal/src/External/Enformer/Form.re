module Config = {
  type t = {validator: option(unit => unit)};

  let newConfig = (~validator=None, ()): t => {validator: validator};
};

type t = {
  config: Config.t,
  className: option(string),
  header: option(string),
};

let newForm = () => {config: Config.newConfig(), className: None, header: None};
let render = (~_form: t) => {
  <div> "I'm the rendered form"->ReasonReact.string </div>;
};
