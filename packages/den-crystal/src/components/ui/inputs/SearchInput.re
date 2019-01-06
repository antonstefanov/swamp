Assets.requireCSS("./SearchInput.scss");

let b = Bem.make("SearchInput");
let str = ReasonReact.string;

let value_of_event = (event): string => ReactEvent.Form.target(event)##value;

type state = {inputRef: ref(option(Dom.element))};

let component = ReasonReact.reducerComponent("SearchInput");

let selectInput = ({ReasonReact.state}) =>
  Belt.Option.map(state.inputRef^, DomInterop.select) |> ignore;
let blurInput = ({ReasonReact.state}) =>
  Belt.Option.map(state.inputRef^, DomInterop.blur) |> ignore;
let handleFocus = (_event, self) => selectInput(self);
let setInputRef = (inputRef, {ReasonReact.state}) => {
  state.inputRef := Js.Nullable.toOption(inputRef);
};

let make =
    (
      ~autoFocus=false,
      ~placeholder="Search",
      ~onChange: string => unit,
      ~value: string,
      ~onBlur=?,
      ~onSubmit=?,
      ~className=?,
      _children,
    ) => {
  let handleKeyDown = (event: ReactEvent.Keyboard.t, self) =>
    switch (ReactEvent.Keyboard.key(event)) {
    | "Escape" =>
      /* otherwise the input is cleared on escape */
      ReactEvent.Keyboard.preventDefault(event);
      blurInput(self);
    | "Enter" => Belt.Option.map(onSubmit, submit => submit(value)) |> ignore
    | _ => ()
    };
  let handleInput = event => event |> value_of_event |> onChange;
  let handleBlur = _event =>
    onBlur |> Maybe.map(blur => blur(value)) |> ignore;

  {
    ...component,
    initialState: () => {inputRef: ref(None)},
    didMount: self => autoFocus ? selectInput(self) : (),
    reducer: (_action: unit, _state: state) => ReasonReact.NoUpdate,
    render: self =>
      <input
        autoFocus
        ref={self.handle(setInputRef)}
        className={Cn.make([b.className, className |> Cn.unwrap])}
        value
        type_="search"
        placeholder
        onChange=handleInput
        onFocus={self.handle(handleFocus)}
        onKeyDown={self.handle(handleKeyDown)}
        onBlur=handleBlur
      />,
  };
};
