let component = ReasonReact.reducerComponent("SearchInput");

let value_of_event = (event): string => ReactEvent.Form.target(event)##value;

let make =
    (
      ~onSubmit: option(string => unit)=?,
      ~onChange: option(string => unit)=?,
      ~initialValue="",
      _,
    ) => {
  let handleChange = (event, {ReasonReact.send}) => {
    let value = value_of_event(event);
    send(value);
    switch (onChange) {
    | None => ()
    | Some(change) => change(value)
    };
  };

  let handleKeyDown = (event, {ReasonReact.state}) =>
    if (ReactEvent.Keyboard.key(event) == "Enter") {
      switch (onSubmit) {
      | None => ()
      | Some(submit) => submit(state)
      };
    };

  {
    ...component,
    initialState: () => initialValue,
    reducer: (newText, _text) => ReasonReact.Update(newText),
    render: self =>
      <input
        value={self.state}
        type_="text"
        placeholder="Write something"
        onChange={self.handle(handleChange)}
        onKeyDown={self.handle(handleKeyDown)}
      />,
  };
};
