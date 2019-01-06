open Bootstrap;

include Command.Async.Make({
  open DenSeed;
  open ConfWrite;

  [@bs.deriving abstract]
  type t = {force: bool};

  let command = "init [options]";
  let description = "Initialise swamp working directory";

  let builder = yargs =>
    Args.(
      yargs
      |> Arg.Bool.make(
           ~arg="force",
           ~alias="f",
           ~desc="If you should be asked before overriding config file",
           ~required=Optional,
         )
    );

  let handler = argv =>
    switch (
      ensureDirsExistSimple([
        Kan.Config.Path.Dir.carbon,
        Kan.Config.Path.Dir.crystal,
      ])
    ) {
    | false => Js.Promise.resolve()
    | true =>
      writeConfigAsync(
        ~confirmOverride=!argv->forceGet,
        ~config=Kan.Config.make(),
        ~dir=Kan.Config.Path.Dir.carbon,
        ~filePath=Kan.Config.Path.json,
        (),
      )
      |> Js.Promise.then_(() =>
           writeUserConfigAsync(
             ~confirmOverride=!argv->forceGet,
             ~config=Kan.Config.User.make(),
             ~dir=Kan.Config.Path.Dir.carbon,
             ~filePath=Kan.Config.Path.userJson,
             (),
           )
         )
      |> Js.Promise.then_(() =>
           copyConfigAsync(
             ~confirmOverride=!argv->forceGet,
             ~src=Fs.swampPath("./den-carbon/src/defaultConfigReader.js"),
             ~dest=Kan.Config.Path.js,
             (),
           )
         )
    };
});
