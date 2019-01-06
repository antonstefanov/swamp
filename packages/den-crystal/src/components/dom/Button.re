Assets.requireCSS("./Button.scss");

let b = Bem.make("Button");

type size =
  | Default
  | Sm
  | Lg;

let string_of_size =
  fun
  | Default => None
  | Sm => "sm"->Some
  | Lg => "lg"->Some;

let getBtnClassname =
    (~className, ~accent, ~block, ~outline, ~pill, ~size, ~iconic, ~focus) =>
  Cn.(
    make([
      b.className,
      accent
      |> Style.string_of_accent
      |> (a => b.modifier((outline ? "outline-" : "solid-") ++ a)),
      size |> string_of_size |> b.modifierIfSome,
      pill |> b.modifierIfTrue("pill"),
      block |> b.modifierIfTrue("block"),
      iconic |> b.modifierIfTrue("iconic"),
      focus |> b.modifierIfTrue("focus"),
      className |> unwrap,
    ])
  );

let component = ReasonReact.statelessComponent("Button");

let make =
    (
      ~title: string,
      ~type_="button",
      ~className: option(string)=?,
      ~onClick=?,
      ~accent=Style.Primary,
      ~block=false,
      ~outline=false,
      ~pill=false,
      ~size=Default,
      ~iconic=false,
      ~focus=false,
      ~disabled=false,
      children,
    ) => {
  ...component,
  render: _self =>
    <button
      className={getBtnClassname(
        ~className,
        ~accent,
        ~block,
        ~outline,
        ~pill,
        ~size,
        ~iconic,
        ~focus,
      )}
      type_
      ?onClick
      title
      ariaLabel=title
      disabled>
      ...children
    </button>,
};
