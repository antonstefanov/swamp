module type Options = {
  type t;
  let name: string;
};

module Make = (T: Options) => {
  type status =
    | NotStarted
    | Loading
    | Result(T.t);

  type state = {
    status,
    mounted: ref(bool),
  };

  type action =
    | Load
    | Result(T.t);
  let component = ReasonReact.reducerComponent(T.name);

  let make =
      (~callback: unit => Js.Promise.t(T.t), ~renderChildren, _children) => {
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
};

module String =
  Make({
    type t = DenSeed.AsyncResult.t(string);
    let name = "LoaderString";
  });
