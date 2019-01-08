module type HandlerResult = {type t;};
module Make = (H: HandlerResult) => {
  module type T = {
    type t;
    let command: string;
    let description: string;
    let builder: Yargs.t => Yargs.t;
    let handler: t => H.t;
  };

  module Make = (M: T) : T => {
    type t = M.t;
    let command = M.command;
    let description = M.description;
    let builder = M.builder;
    let handler = M.handler;
  };
};

module Sync =
  Make({
    type t = unit;
  });
module Async =
  Make({
    type t = Js.Promise.t(unit);
  });
module AsyncWithResult =
  Make({
    type t = Js.Promise.t(DenSeed.Shell.Result.t);
  });

module ShellProxy = {
  module type Options = {
    let description: string;
    let readCmd: string;
    let runCmd: string;
    let cwd: option(string);
    let stderrAsProgress: bool;
  };
  module DefaultOptions = {
    let cwd = None;
    let stderrAsProgress = false;
  };
  let jslogger = DenSeed.Logger.JsLogger.make(Info);

  module Make = (M: Options) =>
    AsyncWithResult.Make({
      [@bs.deriving abstract]
      type t;

      let command = M.readCmd ++ " [args..]";
      let description = M.description;

      let builder = yargs =>
        Args.(
          yargs
          |> Pos.Array.make(
               ~arg="args",
               ~desc="Arguments that will be proxied",
             )
        );

      let run = (~args) =>
        DenSeed.Shell.(
          runCommandAsync(
            ~logger=jslogger,
            ~cmd=M.runCmd,
            ~args,
            ~options=Options.make(),
            ~cmdOptions=
              CmdOptions.make(
                ~cwd=?M.cwd,
                ~shell=true,
                ~forwardOutput=true,
                ~stderrAsProgress=M.stderrAsProgress,
                (),
              ),
            (),
          )
        );

      /* the actual command will be ignored and all args will be
         taken raw from the process and propagated */
      let handler = _argv => {
        run(
          ~args=
            DenSeed.Shell.Argv.(
              make()->args->Belt.Option.getWithDefault([||])
            ),
        );
      };
    });
};
