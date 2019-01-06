Assets.requireCSS("./Card.scss");

let component = ReasonReact.statelessComponent("Card");

let str = ReasonReact.string;
let b = Bem.make("Card");

let getClassName = (~className) =>
  Cn.make([b.className, className |> Cn.unwrap]);

let make =
    (
      ~title: string,
      ~href: string,
      ~description: string,
      ~imageSrc: string,
      ~className: option(string)=?,
      children,
    ) => {
  ...component,
  render: _self =>
    <div className={getClassName(~className)}>
      <Link href title className={b.element("img-link")}>
        <Image
          className={b.element("img")}
          src=imageSrc
          alt=title
          size=Ratio16x9
        />
      </Link>
      <div className={b.element("body")}>
        <h2 className={b.element("title")}> {str(title)} </h2>
        <div className={b.element("text")}> {str(description)} </div>
        <div className={b.element("footer")}> ...children </div>
      </div>
    </div>,
};
