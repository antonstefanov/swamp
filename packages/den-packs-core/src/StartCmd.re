open Bootstrap;

include Command.Async.Make({
  [@bs.deriving abstract]
  type t = {watch: bool};
  let command = "start [options]";
  let description = "Start Crystal";

  let runCrystalCmd = cmd =>
    Cmd.(
      runAsync(
        ~cmd,
        ~cmdOptions=
          CmdOptions.make(
            ~cwd=DenSeed.Fs.swampPath("./carbon/node_modules/den-crystal"),
            ~shell=true,
            ~forwardOutput=true,
            (),
          ),
        (),
      )
    );
  let buildCrystal = () => runCrystalCmd("yarn run build");

  let startCrystalWebpack = () => runCrystalCmd("yarn run dev");

  let start = (~watch): Js.Promise.t(unit) =>
    PacksCrawl.updateCrystalData(~watch)
    |> Js.Promise.then_((result: PacksCrawl.cmdConfigs) =>
         switch (result) {
         | Error(err) =>
           Log.error("Could not get configs", err) |> Js.Promise.resolve
         | Ok(_configs) =>
           buildCrystal()
           |> Js.Promise.then_((buildResult: Cmd.Result.t) =>
                (
                  switch (buildResult) {
                  | Error(err) => Log.error("", err)
                  | Ok(_) => startCrystalWebpack() |> ignore
                  }
                )
                |> Js.Promise.resolve
              )
         }
       );

  let handler = argv => start(~watch=argv->watchGet);
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
