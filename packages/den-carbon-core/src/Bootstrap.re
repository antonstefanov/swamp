open DenSeed;

module Log: Logger.T = {
  open Logger;
  let config = Kan.Config.get();
  module Options: LoggerOptions = {
    let level = Kan.Config.(config->logLevelGet);
  };
  module Logger = LevelLogger(ConsoleLogger, Options);
  include Logger;
};

/* required for js consumption */
let getConfig = Kan.Config.get;

let jslogger = Logger.(JsLogger.make(Kan.Config.(get()->logLevelGet)));

module Cmd = {
  include DenSeed.Shell;
  let runAsync =
      (
        ~logger: DenSeed.Logger.JsLogger.t=jslogger,
        ~cmdOptions=CmdOptions.make(),
        ~options=Options.make(),
        ~args: list(string)=[],
        ~cmd: string,
        (),
      ) =>
    DenSeed.Shell.runCommandAsync(
      ~logger,
      ~cmd,
      ~args=Array.of_list(args),
      ~cmdOptions,
      ~options,
      (),
    );

  let relaunchShell = bashLocation =>
    DenSeed.Shell.relaunch(~logger=jslogger, ~bashLocation, ());
};

module Args = DenCarbonCore.Args;
module Command = DenCarbonCore.Command;
module Q = DenCarbonCore.Q;
module Yargs = DenCarbonCore.Yargs;
module Kan = DenCarbonCore.Kan;
module ConfRead = DenCarbonCore.ConfRead;
module ConfWrite = DenCarbonCore.ConfWrite;
