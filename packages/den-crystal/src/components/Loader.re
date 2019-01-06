type status('result) =
  | NotStarted
  | Loading
  | Result('result);

type state = {
  status: status(DenSeed.AsyncResult.t(string)),
  mounted: ref(bool),
};

type action =
  | Load
  | Result(DenSeed.AsyncResult.t(string));

let component = ReasonReact.reducerComponent("Loader");

let make = (~callback, ~renderChildren, _children) => {
  let load = ({ReasonReact.send, ReasonReact.state}) => {
    send(Load);
    callback()
    |> Js.Promise.then_(result => {
         if (state.mounted^) {
           send(Result(result));
         };
         Js.Promise.resolve();
       })
    |> ignore;
  };
  {
    ...component,
    initialState: () => {status: NotStarted, mounted: ref(true)},
    reducer: (action, state) =>
      switch (action) {
      | Load => ReasonReact.Update({...state, status: Loading})
      | Result(result) =>
        ReasonReact.Update({...state, status: Result(result)})
      },
    didMount: self => load(self),
    willUnmount: self => {
      self.state.mounted := false;
    },
    render: self => renderChildren(self.state.status, () => load(self)),
  };
};
