let str = ReasonReact.string;

let renderRoutes = (data: Data.t) => {
  let jsonPacks = data.packs |> Data.toJson;
  let packs = data.packs |> Array.to_list;
  let packsReducedByTag = Lazy.from_val(Data.reduceToTagMap(packs));
  let packsReducedById = Lazy.from_val(Data.reduceToIdMap(packs));
  let packById = id =>
    packsReducedById->Lazy.force_val->Belt.Map.String.get(id);
  let packsByTag = tag =>
    packsReducedByTag
    ->Lazy.force_val
    ->Belt.Map.String.getWithDefault(tag, [||]);

  <Routes packs={data.packs} jsonPacks packById packsByTag />;
};

let component = ReasonReact.statelessComponent("App");

module LoaderData =
  Loader.Make({
    type t = DenSeed.AsyncResult.t(Data.t);
    let name = "LoaderData";
  });

let make = _children => {
  let load = () => Data.dirtyFetch("/static/data.json");

  let renderResult = (status: LoaderData.status, _) =>
    <>
      <Revealer.InContainer fullScreen=true animate=true />
      {switch (status) {
       | NotStarted => <div> {str("Starting...")} </div>
       | Loading => <Loading />
       | Result(result) =>
         switch (result) {
         | Ok(data) => renderRoutes(data)
         | Error((message, stack)) =>
           <Alert accent=Danger>
             <div> {str(message)} </div>
             <div> {str("Could not fetch data")} </div>
             <hr />
             <div>
               {Belt.Option.mapWithDefault(stack, str(""), s => str(s))}
             </div>
           </Alert>
         }
       }}
    </>;
  {
    ...component,
    render: _self => <LoaderData callback=load renderChildren=renderResult />,
  };
};
