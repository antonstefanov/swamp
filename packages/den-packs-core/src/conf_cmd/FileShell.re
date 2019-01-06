open Bootstrap;

let edit = (~editor, path) =>
  Cmd.(
    runAsync(
      ~cmd=editor,
      ~args=[path],
      ~cmdOptions=CmdOptions.make(~shell=true, ~forwardOutput=true, ()),
      (),
    )
  );
