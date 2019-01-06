open Bootstrap;
open ConfWrite;
open DenSeed;

include Command.Async.Make({
  type t = Kan.Config.User.t;

  let command = "update [options]";
  let description = "Update configuration file";

  let builder = yargs =>
    Args.(
      yargs
      |> Arg.Bool.make(
           ~arg="showLogo",
           ~desc="If logo should be shown before each command",
           ~required=Optional,
         )
      |> Arg.Array.make(
           ~arg="cmdDirs",
           ~desc="Dirs to search for commands",
           ~required=Optional,
         )
      |> Arg.Enum.make(
           ~arg="logLevel",
           ~desc="Log Level " ++ LogLevel.possibleValues,
           ~type_=
             Int(
               Optional,
               LogLevel.levels
               |> List.map(LogLevel.int_of_level)
               |> Array.of_list,
               level => LogLevel.level_of_int(level),
             ),
         )
      |> Arg.String.make(
           ~arg="editor",
           ~desc="Editor to use when opening files",
           ~required=Optional,
         )
      |> Arg.String.make(
           ~arg="packsPath",
           ~desc="Path of the packs location",
           ~required=Optional,
         )
      |> Arg.Array.make(
           ~arg="packs",
           ~desc="Packages to install",
           ~required=Optional,
         )
    );

  let handler = argv =>
    writeUserConfigAsync(
      ~confirmOverride=false,
      ~config=
        Kan.Config.User.(
          make(
            ~logLevel=?argv->logLevelGet,
            ~cmdDirs=?argv->cmdDirsGet,
            ~showLogo=?argv->showLogoGet,
            ~editor=?argv->editorGet,
            ~packsPath=?argv->packsPathGet,
            ~packs=?argv->packsGet,
            (),
          )
        ),
      ~dir=Kan.Config.Path.Dir.carbon,
      ~filePath=Kan.Config.Path.userJson,
      (),
    );
});
