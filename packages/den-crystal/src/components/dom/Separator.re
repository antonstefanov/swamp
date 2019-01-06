Assets.requireCSS("./Separator.scss");

let b = Bem.make("Separator");
type direction =
  | X
  | Y;

let string_of_direction = direction =>
  switch (direction) {
  | X => "x"
  | Y => "y"
  };

let getClassName = (~direction, ~accent) =>
  (direction |> string_of_direction)->(b.appendix)
  ++ " "
  ++ (accent |> Style.string_of_theme |> b.modifier);

let component = ReasonReact.statelessComponent("Separator");

let make = (~direction=X, ~accent: Style.theme=Dark, _children) => {
  ...component,
  render: _self => <hr className={getClassName(~direction, ~accent)} />,
};
