let str = ReasonReact.string;

Assets.requireCSS("./Text.scss");
let b = Bem.make("Text");

module Select = {
  let component = ReasonReact.statelessComponent("TextSelect");

  let make = (~value, _children) => {
    ...component,
    render: _self =>
      <span className={b.element("Select")}> {str(value)} </span>,
  };
};
