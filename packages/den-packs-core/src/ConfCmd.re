open Bootstrap;

include Command.Sync.Make({
  [@bs.deriving abstract]
  type t = {
    [@bs.optional]
    command: string,
    interactive: bool,
  };

  let command = "conf [command] [options]";
  let description = "Configuration";

  let builder = yargs =>
    Args.(
      yargs
      ->dir("conf_cmd", dirOptions(~exclude=excludeHelpers, ()))
      ->interactiveArg(~default=false)
    );

  let handle = (~command, ~interactive) => {
    switch (command) {
    | Some(cmd) =>
      if (!interactive) {
        Cmd.UnknownCommand(
          "Unknown command. To use interactive mode use --interactive",
          cmd,
        )
        |> raise;
      }
    | None => ()
    };
    Q.(
      empty()
      |> list(
           ~arg="command",
           ~desc="Select a command from the list",
           ~default="init",
           ~choices=[|"init"|],
         )
      |> make
    )
    |> Js.Promise.then_(answers => {
         let command = answers->commandGet;
         switch (command) {
         | None => Cmd.NoCommand("No command supplied") |> raise
         | Some(cmd) =>
           switch (cmd) {
           | "init" =>
             ConfWrite.writeConfigAsync(
               ~config=Kan.Config.make(),
               ~dir=Kan.Config.Path.Dir.carbon,
               ~filePath=Kan.Config.Path.json,
               (),
             )
           | _ =>
             Cmd.UnknownCommand(
               "Unknown command. Fix Inquirer questions.",
               cmd,
             )
             |> raise
           }
         };
       })
    |> Js.Promise.catch(err => {
         Log.error("Conf error", err);
         Js.Promise.resolve();
       })
    |> ignore;
  };
  let handler = argv =>
    handle(~command=argv->commandGet, ~interactive=argv->interactiveGet);
});
