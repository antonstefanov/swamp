let component = ReasonReact.statelessComponent("Link");

let handleClick = (href, event) =>
  if (!ReactEvent.Mouse.defaultPrevented(event)) {
    ReactEvent.Mouse.preventDefault(event);
    ReasonReact.Router.push(href);
  };

let make =
    (
      ~href: string,
      ~title: string,
      ~className: option(string)=?,
      ~accent=Style.Primary,
      ~block=false,
      ~outline=false,
      ~pill=false,
      ~size=Button.Default,
      ~iconic=false,
      ~focus=false,
      children,
    ) => {
  ...component,
  render: _self =>
    <a
      href
      className={Button.getBtnClassname(
        ~className,
        ~accent,
        ~block,
        ~outline,
        ~pill,
        ~size,
        ~iconic,
        ~focus,
      )}
      onClick={handleClick(href)}
      title
      ariaLabel=title>
      ...children
    </a>,
};
