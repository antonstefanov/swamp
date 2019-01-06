Assets.requireCSS("./Revealer.scss");

module Standalone = {
  type retainedProps = {animate: bool};

  type animating =
    | NotStarted
    | InProgress
    | Ended;

  type state = {
    animating,
    timeoutId: ref(option(Js.Global.timeoutId)),
  };

  type action =
    | Start
    | Stop
    | End;

  let clearTimeout = timeoutId =>
    Belt.Option.map(
      timeoutId^,
      id => {
        Js.Global.clearTimeout(id);
        timeoutId := None;
      },
    )
    |> ignore;

  let setTimeout = (timeoutId, timeout, callback) => {
    clearTimeout(timeoutId);
    timeoutId := Some(Js.Global.setTimeout(callback, timeout));
  };
  let startAnimate = ({ReasonReact.send}, ()) => {
    send(Start);
  };
  let stopAnimate = ({ReasonReact.send}, ()) => {
    send(Stop);
  };
  let endAnimate = ({ReasonReact.send}, ()) => {
    send(End);
  };
  let reducer = (action, state: state) => {
    switch (action) {
    | Start =>
      ReasonReact.UpdateWithSideEffects(
        {...state, animating: InProgress},
        self => setTimeout(self.state.timeoutId, 2000, endAnimate(self)),
      )
    | Stop =>
      ReasonReact.UpdateWithSideEffects(
        {...state, animating: NotStarted},
        self => clearTimeout(self.state.timeoutId),
      )
    | End =>
      ReasonReact.UpdateWithSideEffects(
        {...state, animating: Ended},
        self => clearTimeout(self.state.timeoutId),
      )
    };
  };

  let component = ReasonReact.reducerComponentWithRetainedProps("Revealer");

  let str = ReasonReact.string;

  let b = Bem.make("Revealer");
  let bLeft = Bem.make(b.element("left"));
  let bRight = Bem.make(b.element("right"));

  let renderRevealer = animate =>
    <div
      className={Cn.make([
        b.className,
        animate |> b.modifierIfTrue("animate"),
      ])}>
      <div
        className={Cn.make([
          bLeft.className,
          animate |> bLeft.modifierIfTrue("animate"),
        ])}
      />
      <div
        className={Cn.make([
          bRight.className,
          animate |> bRight.modifierIfTrue("animate"),
        ])}
      />
    </div>;

  let make =
      (
        ~animate=true,
        ~render:
           (~revealer: ReasonReact.reactElement, ~animating: animating) =>
           ReasonReact.reactElement,
        _children,
      ) => {
    ...component,
    initialState: () => {animating: NotStarted, timeoutId: ref(None)},
    didMount: self => {
      switch (self.state.animating, animate) {
      | (NotStarted, false)
      | (Ended, false) => ()
      | (InProgress, true) => ()
      | (Ended, true)
      | (NotStarted, true) => startAnimate(self, ())
      | (InProgress, false) => stopAnimate(self, ())
      };
    },
    reducer,
    willUnmount: ({state}) => {
      clearTimeout(state.timeoutId);
    },
    retainedProps: {
      animate: animate,
    },
    didUpdate: ({oldSelf, newSelf}) => {
      switch (oldSelf.retainedProps.animate, newSelf.retainedProps.animate) {
      | (false, false) => ()
      | (true, true) => ()
      | (false, true) => startAnimate(newSelf, ())
      | (true, false) => stopAnimate(newSelf, ())
      };
    },
    render: self =>
      (
        switch (self.state.animating) {
        | Ended => ReasonReact.null
        | InProgress => renderRevealer(true)
        | NotStarted => renderRevealer(false)
        }
      )
      |> (revealer => render(~revealer, ~animating=self.state.animating)),
  };
};

module InContainer = {
  let b = Bem.make("RevealerContainer");
  let component = ReasonReact.statelessComponent("RevealerContainer");

  let bLogo = Bem.make(b.element("logo"));
  let make = (~fullScreen, ~animate, _children) => {
    let renderRevealer = revealer =>
      <div
        className={Cn.make([
          b.className,
          fullScreen |> b.modifierIfTrue("fullscreen"),
        ])}>
        revealer
        <Logo
          maximize=true
          className={Cn.make([bLogo.className, bLogo.modifier("animate")])}
        />
      </div>;

    let render = (~revealer, ~animating: Standalone.animating) =>
      switch (animating) {
      | Ended => ReasonReact.null
      | InProgress => renderRevealer(revealer)
      | NotStarted => ReasonReact.null
      };

    {...component, render: _self => <Standalone animate render />};
  };
};
