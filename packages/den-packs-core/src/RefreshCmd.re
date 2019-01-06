open Bootstrap;

include Command.Async.Make({
  [@bs.deriving abstract]
  type t = {watch: bool};
  let command = "refresh [options]";
  let description = "Update Carbon packs Crystal data file";

  let handler = argv =>
    PacksCrawl.updateCrystalData(~watch=argv->watchGet)
    |> Js.Promise.then_((result: PacksCrawl.cmdConfigs) =>
         (
           switch (result) {
           | Error(err) => Log.error("Could not get configs", err)
           | Ok(configs) =>
             Log.info(
               "Updated data file",
               switch (List.length(configs)) {
               | 0 => "0 packs"
               | 1 => "1 pack"
               | len => string_of_int(len) ++ " packs"
               },
             )
           }
         )
         |> Js.Promise.resolve
       );

  let builder = yargs =>
    Args.(
      yargs
      |> Arg.Bool.make(
           ~arg="watch",
           ~alias="w",
           ~desc="Watch for file changes",
           ~required=Default(false),
         )
    );
});
