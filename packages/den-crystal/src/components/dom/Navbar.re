Assets.requireCSS("./Navbar.scss");

type expand = Style.size;
type position = Style.position;

let b = Bem.make("Navbar");
let item = b.element("item");
let bg_of_accent = accent => Style.bg_of_accent(accent);

let getClassName = (~accent, ~theme, ~className) =>
  Cn.make([
    b.className,
    theme |> Style.string_of_theme |> b.appendix,
    accent |> Cn.mapSome(bg_of_accent),
    className |> Cn.unwrap,
  ]);

let component = ReasonReact.statelessComponent("Navbar");
let make =
    (
      ~className: option(string)=?,
      ~accent: option(Style.accent)=?,
      ~theme: Style.theme=Light,
      children,
    ) => {
  ...component,
  render: _self =>
    <header className={getClassName(~accent, ~theme, ~className)}>
      ...children
    </header>,
};
