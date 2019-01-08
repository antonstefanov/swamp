open Bootstrap;

let updateCmd = (~packPath, ~pack, maybeUpdate) =>
  switch (maybeUpdate) {
  | None =>
    Belt.Result.Ok("No update hook for " ++ pack ++ ", skipping")
    |> Js.Promise.resolve
  | Some(update) => PacksShell.updateAsync(~cwd=packPath, update)
  };

include Command.Async.Make({
  [@bs.deriving abstract]
  type t = {
    [@bs.optional]
    packs: array(string),
  };
  let command = "update [packs..]";
  let description = "Update carbon packs";

  let packsPath = Kan.Config.get()->Kan.Config.packsPathGet;

  let updatePack =
      (pack: string): Js.Promise.t(Belt.Result.t(string, DenSeed.Err.t)) => {
    switch (ConfWrite.ensureDirExists(Node.Path.join2(packsPath, pack))) {
    | ErrorNoRights(_path, err)
    | ErrorDoesNotExist(_path, err)
    | Error(_path, err) => Belt.Result.Error(err) |> Js.Promise.resolve
    | AlreadyExists(packPath)
    | Ok(packPath) =>
      pack
      |> PacksCrawl.getCmdHooks
      |> Js.Promise.then_((result: PacksCrawl.cmdHooks) =>
           switch (result) {
           | Error(err) => Belt.Result.Error(err) |> Js.Promise.resolve
           | Ok(maybeHooks) =>
             switch (maybeHooks) {
             | None =>
               Belt.Result.Ok("No hooks for " ++ pack ++ ", skipping")
               |> Js.Promise.resolve
             | Some(hooks) => updateCmd(~packPath, ~pack, hooks.update)
             }
           }
         )
    };
  };

  let updatePacks = (packs: option(array(string))): Js.Promise.t(unit) =>
    switch (ConfRead.User.read()) {
    | Error(err) =>
      Log.error("Could not read config", err) |> Js.Promise.resolve
    | Ok(config) =>
      packs
      |> Maybe.withDefault(
           config->Kan.Config.User.packsGet->Belt.Option.getWithDefault([||]),
         )
      |> Array.map(updatePack)
      |> Js.Promise.all
      |> Js.Promise.then_(results =>
           results
           |> Array.to_list
           |> DenSeed.AsyncResult.allOrNone([])
           |> (
             results =>
               switch (results) {
               | Error(err) =>
                 Log.error("Error during update", err) |> Js.Promise.resolve
               | Ok(updateResults) =>
                 Log.debug("Update results", updateResults);
                 Log.info(
                   "Update hooks successful",
                   "updated: "
                   ++ (updateResults |> List.length |> string_of_int)
                   ++ " packs",
                 );
                 Js.Promise.resolve();
               /* config
                  ->Kan.Config.User.packsGet
                  ->Belt.Option.getWithDefault([||])
                  ->Belt.Set.String.fromArray
                  ->Belt.Set.String.mergeMany(packs)
                  ->Belt.Set.String.toArray
                  |> Kan.Config.User.packsSet(config);
                  config |> ConfWrite.writeJsonConfig(Kan.Config.Path.userJson); */
               }
           )
         )
    };
  let handler = argv => updatePacks(argv->packsGet);
  let builder = yargs =>
    Args.(yargs |> Pos.Array.make(~arg="packs", ~desc="Packages to update"));
});
