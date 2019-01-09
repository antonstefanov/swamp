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
    fn(r) |> Js.Promise.then_(fnResult => fnResult |> Js.Promise.resolve)
  | Belt.Result.Error(err) => Belt.Result.Error(err) |> Js.Promise.resolve
  };

let fetch = path => DenSeed.Fetch.fetch(path) |> result_of_response;

module Response = {
  let text = input =>
    DenSeed.Fetch.Response.text(input) |> result_of_response;
};
