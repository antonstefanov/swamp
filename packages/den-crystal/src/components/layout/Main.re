Assets.requireCSS("./Main.scss");

let component = ReasonReact.statelessComponent("Main");

let make = children => {
  ...component,
  render: _self => <div id="main" className="main"> ...children </div>,
};
