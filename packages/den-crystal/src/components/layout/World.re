Assets.requireCSS("./World.scss");

/* let isWebpSupported = Browsers.hasWebpSupport();
   let className = "World " ++ (isWebpSupported ? "webp" : "no-webp"); */

let component = ReasonReact.statelessComponent("World");
let make = children => {
  ...component,
  render: _self => <div className="World"> ...children </div>,
};
