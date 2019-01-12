let str = ReasonReact.string;

type route =
  | Home
  | Carbon(string)
  | PacksByTag(string)
  | Search(string)
  | NotFound;

let mapUrlToRoute = (url: ReasonReact.Router.url) =>
  switch (url.path) {
  | [] => Home
  | ["carbon", "tag", tag] => PacksByTag(tag)
  | ["carbon", "pack", id] => Carbon(id)
  | ["search"]
  | ["carbon", "search"] => Search("")
  | ["search", search]
  | ["carbon", "search", search] =>
    Search(search |> Js.Global.decodeURIComponent)
  | _ => NotFound
  };

let pushSearch = search =>
  ReasonReact.Router.push(
    "/carbon/search/" ++ Js.Global.encodeURIComponent(search),
  );

module SearchLayout = {
  let component = ReasonReact.statelessComponent("SearchLayout");
  let goSearch = () => pushSearch("");
  let make = children => {
    ...component,
    render: _self =>
      <>
        <CarbonSearchButton href="/carbon/search/" goSearch />
        <div> ...children </div>
      </>,
  };
};

let scrollToTop = () => DomInterop.(scrollTo(window, 0, 0));

module Listener = {
  type state = route;

  type action =
    | ChangeRoute(route);

  let reducer = (action, _state) =>
    switch (action) {
    | ChangeRoute(route) => ReasonReact.Update(route)
    };
  let component = ReasonReact.reducerComponent("RoutesListener");
  let make = (~renderRoute, _children) => {
    ...component,
    reducer,
    initialState: () =>
      ReasonReact.Router.dangerouslyGetInitialUrl() |> mapUrlToRoute,
    didMount: self => {
      let watchId =
        ReasonReact.Router.watchUrl(url =>
          self.send(ChangeRoute(url |> mapUrlToRoute))
        );
      self.onUnmount(() => ReasonReact.Router.unwatchUrl(watchId));
    },
    render: self => renderRoute(self.state),
  };
};

let component = ReasonReact.statelessComponent("Routes");

let make = (~packById, ~packsByTag, ~packs, ~jsonPacks, _children) => {
  ...component,
  render: _self => {
    <Listener
      renderRoute={
        fun
        | Home => <SearchLayout> <PageHome packs /> </SearchLayout>
        | Search(search) => <PageCarbonSearch jsonPacks search pushSearch />
        | Carbon(id) =>
          <SearchLayout> <PageCarbon pack={packById(id)} id /> </SearchLayout>
        | PacksByTag(tag) =>
          <SearchLayout>
            <PagePacksByTag packs={packsByTag(tag)} tag />
          </SearchLayout>
        | NotFound => <PageNotFound />
      }
    />;
  },
};
