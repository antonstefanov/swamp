Assets.requireCSS("./Tags.scss");

let str = ReasonReact.string;

let b = Bem.make("Tags");

let component = ReasonReact.statelessComponent("Tags");

let make = (~tags, ~getLink, _children) => {
  let renderTag = text =>
    <Link
      key=text title=text className={b.element("item")} href={getLink(text)}>
      {str(text)}
    </Link>;

  {
    ...component,
    render: _self =>
      <div className={b.className}>
        {tags |> Array.map(renderTag) |> ReasonReact.array}
      </div>,
  };
};
