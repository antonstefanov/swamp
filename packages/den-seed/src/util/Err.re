type t = (string, option(string));
[@bs.get] [@bs.return undefined_to_opt]
external code: Js.Exn.t => option(string) = "code";

let message = (~default="Unknown error occurred", err) =>
  err->Js.Exn.message->Belt.Option.getWithDefault(default);

let stack = Js.Exn.stack;

let make = (~default=?, err) => (err |> message(~default?), err |> stack);

external exn_of_promise_err: Js.Promise.error => Js.Exn.t = "%identity";
