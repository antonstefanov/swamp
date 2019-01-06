let str = ReasonReact.string;

let value_of_event = (event): string => ReactEvent.Form.target(event)##value;

module type T = {
  type t;
  let name: string;
  let inputClassName: string;
  let fuseOptions: Fuse.Options.t;
  let render:
    (
      ~input: ReasonReact.reactElement,
      ~items: array(t),
      ~searchValue: string
    ) =>
    ReasonReact.reactElement;
};

module Make = (T: T) => {
  type retainedProps = {search: string};
  type state = {
    searchValue: string,
    searchFocused: bool,
    items: array(T.t),
  };
  type action =
    | UpdateItems(array(T.t))
    | Search(string)
    | FocusSearch
    | BlurSearch;

  let component = ReasonReact.reducerComponentWithRetainedProps(T.name);

  let handleFocus = (_event, {ReasonReact.send}) => send(FocusSearch);
  let handleBlur = (_event, {ReasonReact.send}) => send(BlurSearch);

  let make =
      (
        ~items: array(T.t),
        ~search="",
        ~autoFocus=false,
        ~placeholder="Search",
        ~onBlur: option(string => unit)=?,
        ~onSubmit: option(string => unit)=?,
        _children,
      ) => {
    let fuse = Fuse.make(items, T.fuseOptions);
    let searchItems = searchValue => Fuse.search(fuse, searchValue);
    let filterItems = ({ReasonReact.send, ReasonReact.state}) =>
      UpdateItems(searchItems(state.searchValue)) |> send;
    let handleChange = (value, {ReasonReact.send}) => Search(value) |> send;

    {
      ...component,
      initialState: () => {items, searchValue: search, searchFocused: false},
      didMount: filterItems,
      reducer: (action, state) =>
        switch (action) {
        | UpdateItems(nextItems) =>
          ReasonReact.Update({...state, items: nextItems})
        | Search(searchValue) =>
          ReasonReact.UpdateWithSideEffects(
            {...state, searchValue},
            filterItems,
          )
        | FocusSearch => ReasonReact.Update({...state, searchFocused: true})
        | BlurSearch => ReasonReact.Update({...state, searchFocused: false})
        },
      retainedProps: {
        search: search,
      },
      didUpdate: ({oldSelf, newSelf}) =>
        if (oldSelf.retainedProps.search != newSelf.retainedProps.search) {
          newSelf.send(Search(newSelf.retainedProps.search));
        },
      render: self => {
        T.render(
          ~input=
            <SearchInput
              onChange={self.handle(handleChange)}
              value={self.state.searchValue}
              autoFocus
              placeholder
              ?onBlur
              ?onSubmit
            />,
          ~items=self.state.items,
          ~searchValue=self.state.searchValue,
        );
      },
    };
  };
};
