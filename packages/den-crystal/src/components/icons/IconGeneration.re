let b = Bem.make("Icon");
let icon_svg_class = b.element("svg");

type align =
  | Align;
type animation =
  | SpinSlow
  | SpinNormal
  | SpinFast
  | Pulse;
let string_of_animation = spin =>
  switch (spin) {
  | SpinSlow => "a-spin-slow"
  | SpinNormal => "a-spin-normal"
  | SpinFast => "a-spin-fast"
  | Pulse => "a-pulse"
  };
type size =
  | Sm
  | Md
  | Lg
  | Ulg
  | Xl;

let string_of_size = size =>
  switch (size) {
  | Sm => "sm"
  | Md => "md"
  | Lg => "lg"
  | Ulg => "ulg"
  | Xl => "xl"
  };

let getGeneratedIconClass =
    (
      ~name: string,
      ~className: option(string)=?,
      ~size: option(size)=?,
      ~animation: option(animation)=?,
      ~align=false,
      ~spaced=false,
      ~maximize=false,
      ~soloMax=false,
      ~contour=false,
      ~stroke=false,
      ~hover=false,
      ~animate=false,
      ~transparent=false,
      (),
    ) =>
  Cn.(
    make([
      b.className,
      name |> b.appendix,
      align |> b.modifierIfTrue("align"),
      spaced |> b.modifierIfTrue("spaced"),
      maximize |> b.modifierIfTrue("maximize"),
      soloMax |> b.modifierIfTrue("solo-max"),
      stroke |> b.modifierIfTrue("stroke"),
      contour |> b.modifierIfTrue("countour"),
      transparent |> b.modifierIfTrue("transparent"),
      animate |> b.modifierIfTrue("animate"),
      hover |> b.modifierIfTrue("hover"),
      size |> Maybe.map(string_of_size) |> b.modifierIfSome,
      animation |> Maybe.map(string_of_animation) |> b.modifierIfSome,
      className |> unwrap,
    ])
  );

let rotateIfSome = rotate =>
  switch (rotate) {
  | Some(deg) => "rotate(" ++ string_of_int(deg) ++ "deg)"
  | None => ""
  };

let getTransform = (~rotate=?, ~flipX, ~flipY) =>
  Cn.(
    make([
      rotate |> rotateIfSome,
      "scaleX(-1)"->(ifTrue(flipX)),
      "scaleY(-1)"->(ifTrue(flipY)),
    ])
  );

let getStyle =
    (
      ~color: option(string)=?,
      ~rotate: option(int)=?,
      ~flipX=false,
      ~flipY=false,
      (),
    ) =>
  ReactDOMRe.Style.make(
    ~fill=?color,
    ~transform=getTransform(~rotate?, ~flipX, ~flipY),
    (),
  );
