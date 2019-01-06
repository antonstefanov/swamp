Assets.requireCSS("./Page.scss");
let str = ReasonReact.string;

let b = Bem.make("Page");

module Title = {
  let component = ReasonReact.statelessComponent("PageTitle");

  let make = children => {
    ...component,
    render: _self => <h1> ...children </h1>,
  };
};

module SubTitle = {
  let component = ReasonReact.statelessComponent("PageSubTitle");

  let make = children => {
    ...component,
    render: _self => <h2> ...children </h2>,
  };
};

module Description = {
  let component = ReasonReact.statelessComponent("PageDescription");

  let make = children => {
    ...component,
    render: _self =>
      <div className={b.element("Description")}> ...children </div>,
  };
};
