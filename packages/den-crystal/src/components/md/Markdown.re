let str = ReasonReact.string;

let fetch = path =>
  path |> BetterFetch.fetchText |> BetterFetch.thenFlatMap(Marked.parse);

let dirtyFetch = Persist.CacheAsync.make(fetch);

let dangerousHtml: string => Js.t('a) = html => {"__html": html};

let component = ReasonReact.statelessComponent("Markdown");

module Loader = Loader.String;

let make = (~path: string, _children) => {
  let load = () => dirtyFetch(path);

  let renderResult = (status: Loader.status, _) =>
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
