open Bootstrap;

let addCmd = (~packPath, ~pack, maybeAdd) =>
  switch (maybeAdd) {
  | None =>
    Belt.Result.Ok("No add hook for " ++ pack ++ ", skipping")
    |> Js.Promise.resolve
  | Some(add) => PacksShell.installAsync(~cwd=packPath, add)
  };

include Command.Async.Make({
  [@bs.deriving abstract]
  type t = {packs: array(string)};
  let command = "add <packs..>";
  let description = "Install carbon packs";

  let packsPath = Kan.Config.get()->Kan.Config.packsPathGet;

  let addPack =
      (pack: string): Js.Promise.t(Belt.Result.t(string, DenSeed.Err.t)) => {
    switch (ConfWrite.ensureDirExists(Node.Path.join2(packsPath, pack))) {
    | ErrorNoRights(_path, err)
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
             | Some(hooks) => addCmd(~packPath, ~pack, hooks.add)
             }
           }
         )
    };
  };

  let addPacks = (packs: array(string)): Js.Promise.t(unit) =>
    switch (ConfRead.User.read()) {
    | Error(err) =>
      Log.error("Could not read config", err) |> Js.Promise.resolve
    | Ok(config) =>
      packs
      |> Array.map(addPack)
      |> Js.Promise.all
      |> Js.Promise.then_(results =>
           results
           |> Array.to_list
           |> DenSeed.AsyncResult.allOrNone([])
           |> (
             results =>
               switch (results) {
               | Error(err) =>
                 Log.error("Error during install", err) |> Js.Promise.resolve
               | Ok(installResults) =>
                 Log.debug("Install results", installResults);
                 Log.info(
                   "Install hooks ended",
                   "Adding packs to user config.",
                 );
                 config
                 ->Kan.Config.User.packsGet
                 ->Belt.Option.getWithDefault([||])
                 ->Belt.Set.String.fromArray
                 ->Belt.Set.String.mergeMany(packs)
                 ->Belt.Set.String.toArray
                 |> Kan.Config.User.packsSet(config);
                 config |> ConfWrite.writeJsonConfig(Kan.Config.Path.userJson);
               }
           )
         )
    };
  let handler = argv => addPacks(argv->packsGet);
  let builder = yargs =>
    Args.(yargs |> Pos.Array.make(~arg="packs", ~desc="Packages to install"));
});
