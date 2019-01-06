type style =
  | Default
  | Unstyled;

let string_of_style =
  fun
  | Default => "default"
  | Unstyled => "unstyled";

let b = Bem.make("Ul");

let getClassName = (~style, ~className) =>
  Cn.make([
    b.className,
    style |> string_of_style |> b.modifier,
    className |> Cn.unwrap,
  ]);

let component = ReasonReact.statelessComponent("Ul");

let make = (~style=Default, ~className=?, children) => {
  ...component,
  render: _self =>
    <ul className={getClassName(~style, ~className)}> ...children </ul>,
};
