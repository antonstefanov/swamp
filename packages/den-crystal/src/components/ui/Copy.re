Assets.requireCSS("./Copy.scss");
let b = Bem.make("Copy");

let str = ReasonReact.string;

type state = {
  elementRef: ref(option(Dom.element)),
  highlight: bool,
};
type action =
  | Highlight
  | Unhighlight;

let setRef = (elementRef, {ReasonReact.state}) => {
  state.elementRef := Js.Nullable.toOption(elementRef);
};

let select = ({ReasonReact.state}) =>
  Belt.Option.map(state.elementRef^, DomInterop.select) |> ignore;

let handleSelect = (_, self) => select(self);

let setTimeout = Js.Global.setTimeout;
let handleCopy = (_, self) => {
  select(self);
  DomInterop.(document->execCommand("copy"));
  self.send(Highlight);
  setTimeout(() => self.send(Unhighlight), 100) |> ignore;
};

let component = ReasonReact.reducerComponent("Copy");

let make =
    (
      ~value,
      ~title="Copy",
      ~copyOnKeyDown: option(ReactEvent.Keyboard.t => bool)=?,
      ~size=Button.Sm,
      ~outline=true,
      ~accent=?,
      ~pill=?,
      _children,
    ) => {
  let handleKeyDown =
    copyOnKeyDown
    |> Maybe.map((onKeyDown, event: ReactEvent.Keyboard.t, self) =>
         onKeyDown(event) ? handleCopy(event, self) : ()
       );
  {
    ...component,
    initialState: () => {elementRef: ref(None), highlight: false},
    reducer: (action: action, state: state) =>
      switch (action) {
      | Highlight => ReasonReact.Update({...state, highlight: true})
      | Unhighlight => ReasonReact.Update({...state, highlight: false})
      },
    render: self =>
      <>
        <Button
          className={b.element("button")}
          title
          onClick={self.handle(handleCopy)}
          size
          outline={self.state.highlight ? !outline : outline}
          ?accent
          ?pill>
          {str(title)}
        </Button>
        {handleKeyDown
         |> Maybe.map(onKeyDown =>
              <KeyListener onKeyDown={self.handle(onKeyDown)} listen=true />
            )
         |> Maybe.withDefault(ReasonReact.null)}
        <div className={b.element("input-container")}>
          <input
            className={b.element("input")}
            ref={self.handle(setRef)}
            onFocus={self.handle(handleSelect)}
            value
            readOnly=true
            tabIndex=(-1)
          />
        </div>
      </>,
  };
};
