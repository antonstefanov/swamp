let str = ReasonReact.string;

Assets.requireCSS("./CodeCopy.scss");
let b = Bem.make("CodeCopy");

module Input = {
  let bInput = Bem.make(b.element("input"));

  let component = ReasonReact.statelessComponent("CodeInput");

  let make = (~value, _children) => {
    ...component,
    render: _self =>
      <input className={bInput.className} value readOnly=true />,
  };
};

let component = ReasonReact.statelessComponent("CodeCopy");

let make =
    (
      ~value,
      ~title=?,
      ~copyOnKeyDown: option(ReactEvent.Keyboard.t => bool)=?,
      _children,
    ) => {
  ...component,
  render: _self =>
    <div className={b.className}>
      <Copy ?title value ?copyOnKeyDown />
      <Input value />
    </div>,
};
