type t('a) = Belt.Result.t('a, Err.t);
type promiseT('a) = Js.Promise.t(t('a));

let ofJsError = (err: Js.Exn.t) => Belt.Result.Error(Err.make(err));
let ofJs = (result: Belt.Result.t('a, Js.Exn.t)) =>
  switch (result) {
  | Ok(r) => Belt.Result.Ok(r)
  | Error(err) => ofJsError(err)
  };
let resolveOfJs = result => result |> ofJs |> Js.Promise.resolve;

let make = (fn, arg) =>
  fn(arg)
  |> Promisify.make
  |> Js.Promise.then_((result: Promisify.t(unit)) =>
       result |> ofJs |> Js.Promise.resolve
     );

let make2 = (fn, arg) =>
  fn(arg)
  |> Promisify.make2
  |> Js.Promise.then_((result: Promisify.t('a)) =>
       result |> ofJs |> Js.Promise.resolve
     );

let rec allOrNone = (unwrapped: list('a), fileResults: list(t('a))) =>
  switch (fileResults) {
  | [] => Belt.Result.Ok(unwrapped |> List.rev)
  | [x, ...xs] =>
    switch (x) {
    | Ok(fileResult) => allOrNone([fileResult, ...unwrapped], xs)
    | Error(error) => Error(error)
    }
  };
