open Bootstrap;

module type Config = {
  let command: string;
  let description: string;
  let pathDescription: string;
  let pathRequired: Args.Arg.required(string);
};

module Make = (Config: Config) : Command.AsyncWithResult.T => {
  [@bs.deriving abstract]
  type t = {
    editor: string,
    path: string,
  };

  let command = Config.command ++ " [options]";
  let description = Config.description;

  let builder = yargs =>
    Args.(
      yargs
      |> Arg.String.make(
           ~arg="editor",
           ~desc="Editor to use when opening files",
           ~required=Default(Kan.Config.get()->Kan.Config.editorGet),
         )
      |> Arg.String.make(
           ~arg="path",
           ~desc=Config.pathDescription,
           ~required=Config.pathRequired,
         )
    );

  let handler = argv =>
    FileShell.edit(~editor=argv->editorGet, argv->pathGet);
};
