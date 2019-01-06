module CmdOptions = {
  [@bs.deriving abstract]
  type _t = {
    /* Current working directory of the child process */
    [@bs.optional]
    cwd: string,
    /* Environment key-value pairs */
    [@bs.optional]
    env: Js.Dict.t(string),
    /* If true, runs command inside of a shell. Uses '/bin/sh' on UNIX, and 'cmd.exe' on Windows. */
    [@bs.optional]
    shell: bool,
    /* If true, the output of the command will be forwarded */
    [@bs.optional]
    forwardOutput: bool,
    /* Sets the user identity of the process. */
    [@bs.optional]
    uid: int,
    /* Sets the group identity of the process. */
    [@bs.optional]
    gid: int,
    /* Prepare child to run independently of its parent process. */
    [@bs.optional]
    detached: bool,
  };
  type t = _t;
  let make = _t;
};

module Options = {
  [@bs.deriving abstract]
  type _t = {
    onOutData: string => unit,
    onMessage: string => unit,
    onErrorData: string => unit,
  };
  type t = _t;
  let noop = _ => ();
  let make = (~onOutData=noop, ~onMessage=noop, ~onErrorData=noop, ()) =>
    _t(~onOutData, ~onMessage, ~onErrorData);
};

[@bs.deriving abstract]
type runCommandResult = {
  hasErrors: bool,
  result: string,
  errors: string,
};

[@bs.module "./cmd.js"]
external _runCommandAsync:
  (. Logger.JsLogger.t, string, array(string), Options.t, CmdOptions.t) =>
  Js.Promise.t(runCommandResult) =
  "runAsync";

module Result = {
  type completed =
    | WithErrors(string, string)
    | NoErrors(string);

  type t = AsyncResult.t(completed);
  let make = (commandResult: runCommandResult) =>
    switch (commandResult->hasErrorsGet, commandResult->resultGet) {
    | (true, result) => WithErrors(result, commandResult->errorsGet)
    | (false, result) => NoErrors(result)
    };

  let toStringAsyncResult = (result: t): AsyncResult.t(string) =>
    result->Belt.Result.map(
      fun
      | WithErrors(r, _)
      | NoErrors(r) => r,
    );
};

let runCommandAsync =
    (
      ~logger: Logger.JsLogger.t,
      ~cmd: string,
      ~args: array(string),
      ~options: Options.t,
      ~cmdOptions: CmdOptions.t,
      (),
    )
    : Js.Promise.t(Result.t) =>
  _runCommandAsync(. logger, cmd, args, options, cmdOptions)
  |> Js.Promise.then_(result =>
       Belt.Result.Ok(Result.make(result)) |> Js.Promise.resolve
     )
  |> Js.Promise.catch(err =>
       Js.Promise.resolve(
         Belt.Result.Error(err |> Err.exn_of_promise_err |> Err.make),
       )
     );

let relaunch = (~logger: Logger.JsLogger.t, ~bashLocation: string, ()) =>
  runCommandAsync(
    ~logger,
    ~cmd={j|source $bashLocation|j},
    ~args=[||],
    ~options=Options.make(),
    ~cmdOptions=CmdOptions.make(~shell=true, ()),
    (),
  );

module Argv = {
  type t = array(string);
  let make = () => Node.Process.argv;
  let execNodePath = argv => argv[0];
  let execFilePath = argv => argv[1];
  let command = argv => argv |> Array.length > 2 ? Some(argv[2]) : None;
  let args = argv =>
    switch (argv |> Array.length) {
    | len when len <= 3 => None
    | len => Some(argv->Array.sub(3, len - 3))
    };
};

module Args = {
  let make = (args: list(option(string))): list(string) =>
    args
    |> List.filter(Belt.Option.isSome)
    |> Lst.fold_right(
         (maybeArg, args) =>
           switch (maybeArg) {
           | None => args
           | Some(arg) => [arg, ...args]
           },
         [],
       );

  let ifTrue = (arg: string, maybe: bool) => maybe ? Some(arg) : None;
};

exception UnknownCommand(string, string);
exception NoCommand(string);
