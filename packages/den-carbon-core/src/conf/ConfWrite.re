open DenSeed;

let ensureDirExists = path => Fs.mkdirSync(path);

let ensureDirsExist = dirs => {
  let rec aux = (paths, results) =>
    switch (paths) {
    | [] => results
    | [x, ...xs] => aux(xs, [ensureDirExists(x), ...results])
    };
  aux(dirs, []);
};

let traceDirResult = (result: Fs.WriteResult.t) => {
  switch (result) {
  | Error(path, err) => Bootstrap.Log.error({j|"Err ($path)"|j}, err)
  | ErrorDoesNotExist(_path, err) =>
    Bootstrap.Log.error("Dir does not exist", err)
  | ErrorNoRights(_path, err) => Bootstrap.Log.error("No rights", err)
  | Ok(path) => Bootstrap.Log.info("Ok", path)
  | AlreadyExists(path) => Bootstrap.Log.debug("Already exists", path)
  };
  result;
};

let traceDirResults = results => {
  let rec aux = (res: list(Fs.WriteResult.t)) =>
    switch (res) {
    | [] => ()
    | [x, ...xs] =>
      traceDirResult(x) |> ignore;
      aux(xs);
    };
  aux(results);
  results;
};

let ensureDirExistSimple = dir =>
  ensureDirExists(dir)->traceDirResult->Fs.WriteResult.exists;

let ensureDirsExistSimple = dirs =>
  ensureDirsExist(dirs)
  ->traceDirResults
  ->Belt.List.every(Fs.WriteResult.exists);

let confirmText = path =>
  Chalk.yellow("File Already Exists! ")
  ++ "A file with path "
  ++ Chalk.cyan(path)
  ++ " already exists. Override?";

let fileExistsConfirm = (~confirm, ~cancel, filePath) =>
  Q.Ask.Confirm.make(false, ~desc=confirmText(filePath))
  |> Js.Promise.then_(override => override ? confirm() : cancel());

let traceResult = (result: Fs.WriteResult.t) => {
  switch (result) {
  | Error(_path, err) => Bootstrap.Log.error("Err", err)
  | ErrorDoesNotExist(_path, err) =>
    Bootstrap.Log.error("Dir does not exist", err)
  | ErrorNoRights(_path, err) => Bootstrap.Log.error("No rights", err)
  | Ok(path) => Bootstrap.Log.info("Wrote", path)
  | AlreadyExists(path) =>
    Bootstrap.Log.error("File already exists and can't override", path)
  };
  Js.Promise.resolve();
};

let writeJsonConfig = (filePath, config) =>
  config |> Fs.writeJsonAsync(filePath) |> Js.Promise.then_(traceResult);

let copyJsConfig = (src, dest) =>
  Fs.copyFileAsync(src, dest) |> Js.Promise.then_(traceResult);

let writeJsonConfigWithConfirm = (~confirmOverride, path, config) =>
  Q.Decorator.withConfirm(
    ~confirmDefault=false,
    ~confirmOverride,
    ~confirmText,
    ~path,
    ~confirm=() => writeJsonConfig(path, config),
    ~cancel=() => Bootstrap.Log.info("Canceled", "") |> Js.Promise.resolve,
  );

let writeConfigAsync = (~confirmOverride=true, ~config, ~dir, ~filePath, ()) =>
  switch (
    ensureDirsExistSimple([
      dir,
      ...Kan.Config.(config->cmdDirsGet |> Array.to_list),
    ])
  ) {
  | false => Js.Promise.resolve()
  | true => writeJsonConfigWithConfirm(~confirmOverride, filePath, config)
  };
let writeUserConfigAsync =
    (~confirmOverride=true, ~config, ~dir, ~filePath, ()) =>
  switch (
    ensureDirsExistSimple([
      dir,
      ...Kan.Config.User.(
           config->cmdDirsGet->Belt.Option.getWithDefault([||])->Array.to_list
         ),
    ])
  ) {
  | false => Js.Promise.resolve()
  | true => writeJsonConfigWithConfirm(~confirmOverride, filePath, config)
  };

let copyJsConfigWithConfirm = (~confirmOverride, src, dest) =>
  Q.Decorator.withConfirm(
    ~confirmDefault=false,
    ~confirmOverride,
    ~confirmText,
    ~path=dest,
    ~confirm=() => copyJsConfig(src, dest),
    ~cancel=() => Bootstrap.Log.info("Canceled", "") |> Js.Promise.resolve,
  );

let copyConfigAsync = (~confirmOverride=true, ~src, ~dest, ()) =>
  copyJsConfigWithConfirm(~confirmOverride, src, dest);

let deleteConfirmText = _path =>
  Chalk.yellow("You are about to delete all config files.")
  ++ " You can use 'conf init' to restore the defaults afterwards."
  ++ " Are you sure? ";

let traceDeleteResult = (result: Fs.FsResult.t(string)) =>
  switch (result) {
  | Error(err) => Bootstrap.Log.error("Err", err)
  | Ok(path) => Bootstrap.Log.info("Deleted", path)
  };

let clearConfigAsync = confirmOverride =>
  Q.Decorator.withDoubleConfirm(
    ~confirmation="ananas",
    ~confirmOverride,
    ~confirmText=deleteConfirmText,
    ~path="",
    ~confirm=
      () =>
        Js.Promise.all2((
          Fs.deleteFileAsync(Kan.Config.Path.json),
          Fs.deleteFileAsync(Kan.Config.Path.js),
        ))
        |> Js.Promise.then_(((json, js)) => {
             traceDeleteResult(json);
             traceDeleteResult(js);
             Js.Promise.resolve();
           }),
    ~cancel=() => Bootstrap.Log.info("Canceled", "") |> Js.Promise.resolve,
  );
