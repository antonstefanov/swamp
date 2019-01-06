let str = ReasonReact.string;

type route =
  | Home
  | Carbon(string)
  | PacksByTag(string)
  | Search(string)
  | NotFound;

type state = {route};

type action =
  | ChangeRoute(route);

let reducer = (action, _state) =>
  switch (action) {
  | ChangeRoute(route) => ReasonReact.Update({route: route})
  };

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
let goSearch = () => pushSearch("");

let data = Data.read();
let packs = Data.packs_of_data(data);
let packsReducedByTag = Lazy.from_val(Data.reduceToTagMap(packs));
let packsReducedById = Lazy.from_val(Data.reduceToIdMap(packs));
let packById = id =>
  packsReducedById->Lazy.force_val->Belt.Map.String.get(id);
let packsByTag = tag =>
  packsReducedByTag
  ->Lazy.force_val
  ->Belt.Map.String.getWithDefault(tag, [||]);
let jsonPacks = data.packs |> Data.toJson;

module SearchLayout = {
  let component = ReasonReact.statelessComponent("SearchLayout");
  let make = children => {
    ...component,
    render: _self =>
      <>
        <CarbonSearchButton href="/carbon/search/" goSearch />
        <div> ...children </div>
      </>,
  };
};

let component = ReasonReact.reducerComponent("App");

let scrollToTop = () => DomInterop.(scrollTo(window, 0, 0));
let make = _children => {
  ...component,
  reducer,
  initialState: () => {route: Home},
  didMount: self => {
    let watchId =
      ReasonReact.Router.watchUrl(url =>
        self.send(ChangeRoute(url |> mapUrlToRoute))
      );
    self.onUnmount(() => ReasonReact.Router.unwatchUrl(watchId));
    /* eval packs after page load */
    Lazy.force_val(packsReducedById) |> ignore;
    Lazy.force_val(packsReducedByTag) |> ignore;
  },
  render: self =>
    <>
      <Revealer.InContainer fullScreen=true animate=true />
      {switch (self.state.route) {
       | Home => <SearchLayout> <PageHome packs={data.packs} /> </SearchLayout>
       | Search(search) => <PageCarbonSearch jsonPacks search pushSearch />
       | Carbon(id) =>
         <SearchLayout> <PageCarbon pack={packById(id)} id /> </SearchLayout>
       | PacksByTag(tag) =>
         <SearchLayout>
           <PagePacksByTag packs={packsByTag(tag)} tag />
         </SearchLayout>
       | NotFound => <PageNotFound />
       }}
    </>,
};
