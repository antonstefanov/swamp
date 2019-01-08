open Bootstrap;

let removeCmd = (~packPath, ~pack, maybeRemove) =>
  switch (maybeRemove) {
  | None =>
    Belt.Result.Ok("No remove hook for " ++ pack ++ ", skipping")
    |> Js.Promise.resolve
  | Some(remove) => PacksShell.installAsync(~cwd=packPath, remove)
  };

include Command.Async.Make({
  [@bs.deriving abstract]
  type t = {packs: array(string)};
  let command = "remove <packs..>";
  let description = "Uninstall carbon packs";

  let packsPath = Kan.Config.get()->Kan.Config.packsPathGet;

  let removePack =
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
               Belt.Result.Ok(
                 "No install config for " ++ pack ++ ", skipping",
               )
               |> Js.Promise.resolve
             | Some(hooks) => removeCmd(~packPath, ~pack, hooks.remove)
             }
           }
         )
    };
  };
  let removePacks = (packs: array(string)): Js.Promise.t(unit) =>
    switch (ConfRead.User.read()) {
    | Error(err) =>
      Log.error("Could not read config", err) |> Js.Promise.resolve
    | Ok(config) =>
      packs
      |> Array.map(removePack)
      |> Js.Promise.all
      |> Js.Promise.then_(results =>
           results
           |> Array.to_list
           |> DenSeed.AsyncResult.allOrNone([])
           |> (
             results =>
               switch (results) {
               | Error(err) =>
                 Log.error("Error during uninstall", err)
                 |> Js.Promise.resolve
               | Ok(uninstallResults) =>
                 Log.debug("Uninstall results", uninstallResults);
                 Log.info(
                   "Uninstall successful",
                   "Removing packs from user config.",
                 );
                 config
                 ->Kan.Config.User.packsGet
                 ->Belt.Option.getWithDefault([||])
                 ->Belt.Set.String.fromArray
                 ->Belt.Set.String.removeMany(packs)
                 ->Belt.Set.String.toArray
                 |> Kan.Config.User.packsSet(config);

                 config |> ConfWrite.writeJsonConfig(Kan.Config.Path.userJson);
               }
           )
         )
    };
  let handler = argv => removePacks(argv->packsGet);
  let builder = yargs =>
    Args.(
      yargs |> Pos.Array.make(~arg="packs", ~desc="Packages to uninstall")
    );
});
