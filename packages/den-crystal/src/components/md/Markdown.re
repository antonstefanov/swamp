let str = ReasonReact.string;
let fetchMd = path =>
  BetterFetch.fetch(path)
  |> Js.Promise.then_(BetterFetch.promiseMap(BetterFetch.Response.text));

let fetch = path =>
  path |> fetchMd |> Js.Promise.then_(BetterFetch.promiseMap(Marked.parse));

let dirtyFetch = Persist.CacheAsync.make(fetch);

let dangerousHtml: string => Js.t('a) = html => {"__html": html};

let component = ReasonReact.statelessComponent("Markdown");

let make = (~path: string, _children) => {
  let load = () => dirtyFetch(path);

  let renderResult = (status: Loader.status(DenSeed.AsyncResult.t(string)), _) =>
    switch (status) {
    | NotStarted => <div> {str("Starting...")} </div>
    | Loading => <Loading />
    | Result(result) =>
      switch (result) {
      | Ok(text) => <div dangerouslySetInnerHTML={dangerousHtml(text)} />
      | Error((message, stack)) =>
        <Alert accent=Danger>
          <div> {str(message)} </div>
          <div>
            {str("Path: ")}
            <a href=path rel="noopener noreferrer" target="_blank">
              {str(path)}
            </a>
          </div>
          <hr />
          <div>
            {Belt.Option.mapWithDefault(stack, str(""), s => str(s))}
          </div>
        </Alert>
      }
    };
  {
    ...component,
    render: _self => <Loader callback=load renderChildren=renderResult />,
  };
};
