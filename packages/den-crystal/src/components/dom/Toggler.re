type state = bool;
type action =
  | Toggle;

let str = ReasonReact.string;
let defaultRenderToggler = (~show: bool, ~onClick: ReactEvent.Mouse.t => unit) =>
  <Button title="Toggle" onClick> {str(show ? "Hide" : "Show")} </Button>;

let component = ReasonReact.reducerComponent("Toggler");
let make =
    (
      ~className=?,
      ~renderToggler=?,
      ~show=false,
      ~alignItems=?,
      ~gap=1,
      children,
    ) => {
  let renderToggler = Maybe.withDefault(defaultRenderToggler, renderToggler);
  let click = ({ReasonReact.send}, _) => send(Toggle);

  {
    ...component,
    initialState: () => show,
    reducer: (action, state) =>
      switch (action) {
      | Toggle => ReasonReact.Update(!state)
      },
    render: self =>
      <Flex ?className direction=Column gap ?alignItems>
        {renderToggler(~onClick=click(self), ~show=self.state)}
        {self.state ? children() : ReasonReact.null}
      </Flex>,
  };
};
