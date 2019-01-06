module User = {
  include Kan.Config.User;
  external ofJson: Js.Json.t => t = "%identity";

  let read = (): Belt.Result.t(t, DenSeed.Err.t) =>
    Kan.Config.Path.userJson
    ->DenSeed.Fs.Json.read
    ->Belt.Result.map(json => json |> ofJson);

  let getPacks = () =>
    Belt.Result.map(read(), config =>
      config->packsGet->Belt.Option.getWithDefault([||])
    );
};
