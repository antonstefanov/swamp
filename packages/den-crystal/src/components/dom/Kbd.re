Assets.requireCSS("./Kbd.scss");

module Kbd = {
  let component = ReasonReact.statelessComponent("Kbd");

  let make = children => {
    ...component,
    render: _self => <kbd> ...children </kbd>,
  };
};

include Kbd;

let cmd = <Kbd> [%raw {|'⌘'|}] </Kbd>;
