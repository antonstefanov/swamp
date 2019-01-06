let str = ReasonReact.string;

Assets.requireCSS("./Alert.scss");
let b = Bem.make("Alert");

let getClassName = (~accent, ~transparent) =>
  Cn.(
    make([
      b.className,
      Bg.getClassName(~accent, ~transparent, ~coloredText=true),
    ])
  );

let component = ReasonReact.statelessComponent("Alert");

let make = (~accent=Style.Danger, ~transparent=false, children) => {
  ...component,
  render: _self =>
    <div className={getClassName(~accent, ~transparent)}> ...children </div>,
};
