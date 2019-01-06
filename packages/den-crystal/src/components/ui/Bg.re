let str = ReasonReact.string;

let b = Bem.make("Bg");

let getClassName = (~accent, ~coloredText, ~transparent) =>
  Cn.(
    make([
      b.className,
      accent
      |> Style.string_of_accent
      |> (
        a =>
          b.modifier(
            ifTrue("color-", coloredText)
            ++ ifTrue("transparent-", transparent)
            ++ a,
          )
      ),
    ])
  );

let component = ReasonReact.statelessComponent("Bg");

let make = (~accent, ~coloredText=true, ~transparent=false, children) => {
  ...component,
  render: _self =>
    <div className={getClassName(~accent, ~coloredText, ~transparent)}>
      ...children
    </div>,
};
