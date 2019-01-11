open Bootstrap;

include Command.Async.Make({
  [@bs.deriving abstract]
  type t = {
    watch: bool,
    port: int,
  };
  let command = "start [options]";
  let description = "Start Crystal";

  let runCrystalCmd = (~args=?, cmd) =>
    Cmd.(
      runAsync(
        ~cmd,
        ~args?,
        ~cmdOptions=
          CmdOptions.make(
            ~cwd=DenSeed.Fs.swampPath("./den-crystal-server"),
            ~shell=true,
            ~forwardOutput=true,
            ~stderrAsProgress=true,
            (),
          ),
        (),
      )
    );

  let dirname: string = Belt.Option.getWithDefault([%bs.node __dirname], ".");

  /* let runLocalCmd = cmd =>
     Cmd.(
       runAsync(
         ~cmd,
         ~cmdOptions=
           CmdOptions.make(
             ~cwd=dirname,
             ~shell=true,
             ~forwardOutput=true,
             ~stderrAsProgress=true,
             (),
           ),
         (),
       )
     ); */

  /* let buildCrystal = () => runCrystalCmd("yarn run build"); */
  let buildCrystal = () =>
    Js.Promise.resolve(Belt.Result.Ok(Cmd.Result.NoErrors("fake build")));

  /* let startCrystal = () => runLocalCmd("yarn run start-crystal"); */
  let startCrystal = port =>
    runCrystalCmd("yarn run serve", ~args=["-l", port->string_of_int]);

  let packsPath = Kan.Config.get()->Kan.Config.packsPathGet;

  let link = () =>
    Cmd.(
      runAsync(
        ~cmd="ln -sfn",
        ~args=[
          packsPath,
          DenSeed.Fs.swampPath("./den-crystal/dist/static/"),
        ],
        ~cmdOptions=
          CmdOptions.make(
            ~cwd=dirname,
            ~shell=true,
            ~forwardOutput=true,
            ~stderrAsProgress=true,
            (),
          ),
        (),
      )
    );

  let start = (~watch, ~port): Js.Promise.t(unit) =>
    PacksCrawl.updateCrystalData(~watch)
    |> Js.Promise.then_((result: PacksCrawl.cmdConfigs) =>
         switch (result) {
         | Error(err) =>
           Log.error("Could not get configs", err) |> Js.Promise.resolve
         | Ok(_configs) =>
           buildCrystal()
           |> Js.Promise.then_((buildResult: Cmd.Result.t) =>
                switch (buildResult) {
                | Error(err) => Log.error("", err) |> Js.Promise.resolve
                | Ok(_) =>
                  link()
                  |> Js.Promise.then_((linkResult: DenSeed.Shell.Result.t) =>
                       switch (linkResult) {
                       | Error(err) =>
                         Log.error("link err", err) |> Js.Promise.resolve
                       | Ok(_) =>
                         startCrystal(port) |> ignore |> Js.Promise.resolve
                       }
                     )
                }
              )
         }
       );

  let handler = argv => start(~watch=argv->watchGet, ~port=argv->portGet);
  let builder = yargs =>
    Args.(
      yargs
      |> Arg.Bool.make(
           ~arg="watch",
           ~alias="w",
           ~desc="Watch for file changes",
           ~required=Default(false),
         )
      |> Arg.Int.make(
           ~arg="port",
           ~alias="p",
           ~desc="Server port",
           ~required=Default(5000),
         )
    );
});
