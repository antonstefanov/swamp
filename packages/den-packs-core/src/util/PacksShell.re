open Bootstrap;

module Result = {
  type t = DenSeed.AsyncResult.t(string);
  let ofShellResult = (resultPromise: Js.Promise.t(DenSeed.Shell.Result.t)) =>
    resultPromise
    |> Js.Promise.then_(result =>
         result
         |> DenSeed.Shell.Result.toStringAsyncResult
         |> Js.Promise.resolve
       );
};

let run = (~cwd: string, cmd: string) =>
  Cmd.(
    runAsync(
      ~cmd,
      ~cmdOptions=
        CmdOptions.make(
          ~shell=true,
          ~cwd,
          ~forwardOutput=true,
          ~stderrAsProgress=true,
          (),
        ),
      (),
    )
  )
  |> Result.ofShellResult;

let installAsync = run;

let uninstallAsync = run;

let updateAsync = run;
