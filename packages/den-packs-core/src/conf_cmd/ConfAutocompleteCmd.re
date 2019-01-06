open Bootstrap;

include Command.AsyncWithResult.Make({
  [@bs.deriving abstract]
  type t = {
    write: bool,
    path: string,
  };

  let command = "autocomplete";
  let description = "Initialise bash autocompletion.";

  let builder = yargs => yargs;

  let handler = _argv => {
    Q.Ask.Confirm.make(true, ~desc="Append to your bash config?")
    |> Js.Promise.then_(write =>
         switch (write) {
         | false =>
           Log.info(
             "Script to console only",
             "You will need to manually add it to your bash config",
           );
           ConfAutocompleteShell.autocomplete(Console);
         | true =>
           Q.Ask.Input.make(
             ~desc="Location of your bash config",
             ~default=DenSeed.Os.(Platform.value |> Bashrc.location),
             (),
           )
           |> Js.Promise.then_(bashLocation =>
                ConfAutocompleteShell.autocomplete(Bash(bashLocation))
                |> Js.Promise.then_((result: Cmd.Result.t) =>
                     switch (result) {
                     | Ok(r) =>
                       /* Cmd.relaunchShell(bashLocation); */
                       Log.info(
                         "Relaunch terminal for the config to take effect",
                         "",
                       );
                       Belt.Result.Ok(r) |> Js.Promise.resolve;
                     | Error(err) =>
                       Belt.Result.Error(err) |> Js.Promise.resolve
                     }
                   )
              )
         }
       );
  };
});
