open Bootstrap;

include Command.AsyncWithResult.Make({
  [@bs.deriving abstract]
  type t = {
    editor: string,
    js: bool,
  };

  let command = "edit [options]";
  let description = "Edit configuration files";

  let builder = yargs =>
    Args.(
      yargs
      |> Arg.String.make(
           ~arg="editor",
           ~desc="Editor to use when opening files",
           ~required=Default(Kan.Config.get()->Kan.Config.editorGet),
         )
      |> Arg.Bool.make(
           ~arg="js",
           ~desc="True for js, false for json",
           ~required=Default(false),
         )
    );

  let handler = argv =>
    FileShell.edit(
      ~editor=argv->editorGet,
      argv->jsGet ? Kan.Config.Path.js : Kan.Config.Path.userJson,
    );
});
