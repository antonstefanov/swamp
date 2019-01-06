module InternalKeyListener = {
  [@bs.send]
  external addKeyboardEventListener:
    (Dom.document, string, ReactEvent.Keyboard.t => unit, bool) => unit =
    "addEventListener";
  [@bs.send]
  external removeKeyboardEventListener:
    (Dom.document, string, ReactEvent.Keyboard.t => unit, bool) => unit =
    "removeEventListener";

  let component = ReasonReact.statelessComponent("InternalKeyListener");

  let make = (~onKeyDown: ReactEvent.Keyboard.t => unit, _children) => {
    ...component,
    didMount: self => {
      addKeyboardEventListener(
        DomInterop.document,
        "keydown",
        onKeyDown,
        false,
      );
      self.onUnmount(() =>
        removeKeyboardEventListener(
          DomInterop.document,
          "keydown",
          onKeyDown,
          false,
        )
      );
    },
    render: _self => ReasonReact.null,
  };
};

let component = ReasonReact.statelessComponent("KeyListener");

let make =
    (~onKeyDown: ReactEvent.Keyboard.t => unit, ~listen=true, _children) => {
  ...component,
  render: _self =>
    listen ? <InternalKeyListener onKeyDown /> : ReasonReact.null,
};
