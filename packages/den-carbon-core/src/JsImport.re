let log = Bootstrap.jslogger;

let dir =
  Bootstrap.Kan.{
    "swamp": Config.Path.Dir.swamp,
    "carbon": Config.Path.Dir.carbon,
    "crystal": Config.Path.Dir.crystal,
  };
let config = Bootstrap.Kan.{"get": Config.get};

let runCmdAsync = Bootstrap.Cmd.runAsync;

let includeOnlyCmd = DenCarbonCore.Args.includeOnlyCmd;
