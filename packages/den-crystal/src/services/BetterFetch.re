let result_of_response = response =>
  response
  |> Js.Promise.then_(result => Belt.Result.Ok(result) |> Js.Promise.resolve)
  |> Js.Promise.catch(error =>
       Belt.Result.Error(
         error |> DenSeed.Err.exn_of_promise_err |> DenSeed.Err.make,
       )
       |> Js.Promise.resolve
     );

let promiseMap = (fn, result) =>
  switch (result) {
  | Belt.Result.Ok(r) =>
    fn(r)
    |> Js.Promise.then_(fnResult =>
         Belt.Result.Ok(fnResult) |> Js.Promise.resolve
       )
  | Belt.Result.Error(err) => Belt.Result.Error(err) |> Js.Promise.resolve
  };

let promiseFlatMap = (fn, result) =>
  switch (result) {
  | Belt.Result.Ok(r) =>
    fn(r) |> Js.Promise.then_(fnResult => fnResult |> Js.Promise.resolve)
  | Belt.Result.Error(err) => Belt.Result.Error(err) |> Js.Promise.resolve
  };

let thenFlatMap = (fn, promise) =>
  promise |> Js.Promise.then_(promiseFlatMap(fn));

let thenMap = (fn, promise) => promise |> Js.Promise.then_(promiseMap(fn));

module Response = {
  let text = input =>
    DenSeed.Fetch.Response.text(input) |> result_of_response;

  let json = input =>
    DenSeed.Fetch.Response.json(input) |> result_of_response;
};

let fetch = path => DenSeed.Fetch.fetch(path) |> result_of_response;

let fetchText = path =>
  fetch(path) |> Js.Promise.then_(promiseFlatMap(Response.text));

let fetchJson = path =>
  fetch(path) |> Js.Promise.then_(promiseFlatMap(Response.json));
