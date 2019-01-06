open Bootstrap;

include Command.Async.Make({
  [@bs.deriving abstract]
  type t = {force: bool};

  let command = "clear [options]";
  let description = "Clear configuration files";

  let builder = yargs =>
    Args.(
      yargs
      |> Arg.Bool.make(
           ~arg="force",
           ~desc="If you should be asked before deleting files",
           ~required=Default(false),
         )
    );

  let handler = argv => ConfWrite.clearConfigAsync(!argv->forceGet);
});
