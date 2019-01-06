Assets.requireCSS("./Image.scss");
let b = Bem.make("Image");
let bImg = Bem.make(b.element("img"));

type size =
  | Ratio1x1
  | Ratio16x9
  | Ratio4x3;

let string_of_size =
  fun
  | Ratio1x1 => "1x1"
  | Ratio16x9 => "16x9"
  | Ratio4x3 => "4x3";

let component = ReasonReact.statelessComponent("Image");

let make = (~className=?, ~src: string, ~alt: string, ~size: size, _children) => {
  ...component,
  render: _self =>
    <div
      className={Cn.make([
        b.className,
        className |> Cn.unwrap,
        size |> string_of_size |> b.modifier,
      ])}>
      <img className={bImg.className} src alt />
    </div>,
};
