Assets.requireCSS("./CarbonSearchButton.scss");

let b = Bem.make("CarbonSearchButton");

let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("CarbonSearchButton");

let make = (~href, ~goSearch, _children) => {
  let onKeyDown = (event: ReactEvent.Keyboard.t) => {
    switch (
      ReactEvent.Keyboard.key(event),
      ReactEvent.Keyboard.metaKey(event),
      ReactEvent.Keyboard.altKey(event),
      ReactEvent.Keyboard.shiftKey(event),
    ) {
    | ("t", false, false, false) =>
      /* it's always a good idea to steal user events */
      ReactEvent.Keyboard.preventDefault(event);
      goSearch();
    | _ => ()
    };
  };
  {
    ...component,
    render: _self =>
      <>
        <KeyListener onKeyDown listen=true />
        <LinkButton href title="Search" className={b.className}>
          {str("(t)earch")}
        </LinkButton>
      </>,
  };
};
