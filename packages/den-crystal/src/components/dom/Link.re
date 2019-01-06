let component = ReasonReact.statelessComponent("Link");

let handleClick = (href, event) =>
  if (!ReactEvent.Mouse.defaultPrevented(event)) {
    ReactEvent.Mouse.preventDefault(event);
    ReasonReact.Router.push(href);
  };

let make =
    (~href: string, ~title: string, ~className: option(string)=?, children) => {
  ...component,
  render: _self =>
    <a href ?className onClick={handleClick(href)} title ariaLabel=title>
      ...children
    </a>,
};
