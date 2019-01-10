open DenSeed;
open Bootstrap;

type filesResult = Fs.FsResult.t(list(Fs.FileResult.t));
/*  get the actual config merged with the user config */
let packsPath = Kan.Config.get()->Kan.Config.packsPathGet;

[@bs.val]
external require: string => DenSeed.Packs.JsInterop.Input.t = "require";

let getImages = (path: string) =>
  Fs.FileResult.(
    readDirUnwrapped(
      f =>
        getFilename(f)
        ->Belt.Option.map(isImg)
        ->Belt.Option.getWithDefault(false),
      path,
    )
  );

type cmdConfig = Belt.Result.t(DenSeed.Packs.Json.t, DenSeed.Err.t);

let getCmdConfig = (path, folderName): Js.Promise.t(cmdConfig) => {
  let cmdDir = Node.Path.join2(path, folderName);
  let defaultReadmePath = Node.Path.join2(cmdDir, "README.md");
  let defaultReadmePathOutput =
    "/static/packs" ++ defaultReadmePath |> Js.String.replace(path, "");
  let imagesDir = Node.Path.join2(cmdDir, "images");
  let imagesDirOutput =
    "/static/packs" ++ imagesDir |> Js.String.replace(path, "");
  let configPath = Node.Path.join2(cmdDir, "cmd.jsonc");

  let validReadme = readme =>
    switch (readme) {
    | None =>
      switch (Fs.exists(defaultReadmePath)) {
      | false =>
        Belt.Result.Error((
          "Mandatory README file does not exist.",
          Some(defaultReadmePath),
        ))
      | true => Belt.Result.Ok(defaultReadmePathOutput)
      }
    | Some(url) => Belt.Result.Ok(url) /* won't check urls */
    };

  let getConfig = (stats, images) =>
    configPath
    ->Fs.Json.read
    ->Belt.Result.flatMap(json =>
        json
        |> DenSeed.Packs.JsInterop.Input.ofJson
        |> (
          config =>
            Belt.Result.map(
              validReadme(config->DenSeed.Packs.JsInterop.Input.readmeGet),
              readmePath =>
              DenSeed.Packs.Json.make(
                ~title=folderName,
                ~description=
                  config->DenSeed.Packs.JsInterop.Input.descriptionGet,
                ~tags=
                  config
                  ->DenSeed.Packs.JsInterop.Input.tagsGet
                  ->(
                      tags => {
                        Array.sort(compare, tags);
                        tags;
                      }
                    ),
                ~images=Array.of_list(images),
                ~readme=readmePath,
                ~hooks=?config->DenSeed.Packs.JsInterop.Input.hooksGet,
                ~created=Fs.Stats.birthtimeMs(stats),
                ~modified=Fs.Stats.mtimeMs(stats),
                (),
              )
            )
        )
      );

  Js.Promise.all2((imagesDir |> getImages, configPath |> DenSeed.Fs.lstat))
  |> Js.Promise.then_(
       (
         (
           imagesResult: DenSeed.Fs.FileResult.readDirUnwrappedResult,
           statsResult: Fs.FsResult.t(Fs.Stats.t),
         ),
       ) =>
       switch (statsResult) {
       | Error(err) => Js.Promise.resolve(Belt.Result.Error(err))
       | Ok(stats) =>
         (
           switch (imagesResult) {
           | Error(err) =>
             Log.warn(folderName ++ ": could not get any images.", err);
             [];
           | Ok(images) =>
             images |> List.map(i => Node.Path.join2(imagesDirOutput, i))
           }
         )
         |> getConfig(stats)
         |> Js.Promise.resolve
       }
     );
};

type cmdConfigs = Belt.Result.t(list(DenSeed.Packs.Json.t), DenSeed.Err.t);
let getCmdConfigs = (path): Js.Promise.t(cmdConfigs) =>
  Fs.FileResult.(readDirUnwrapped(isDir, path))
  |> Js.Promise.then_((result: Fs.FileResult.readDirUnwrappedResult) =>
       switch (result) {
       | Error(err) => Js.Promise.resolve(Belt.Result.Error(err))
       | Ok(folders) =>
         folders
         |> Array.of_list
         |> Array.map(getCmdConfig(path))
         |> Js.Promise.all
         |> Js.Promise.then_(results =>
              results
              |> Array.to_list
              |> Fs.FileResult.allOrNone([])
              |> Js.Promise.resolve
            )
       }
     );

let updateCrystalData =
    (~watch as _watchNotImplemented): Js.Promise.t(cmdConfigs) =>
  packsPath
  |> getCmdConfigs
  |> Js.Promise.then_((result: cmdConfigs) =>
       switch (result) {
       | Error(err) => Belt.Result.Error(err) |> Js.Promise.resolve
       | Ok(configs) =>
         switch (
           DenSeed.Fs.swampPath("./den-crystal/dist/")
           ->ConfWrite.ensureDirExists
         ) {
         /* DenSeed.Fs.swampPath("./den-crystal/src/static/data.json"), */
         | ErrorNoRights(_path, err)
         | ErrorDoesNotExist(_path, err)
         | Error(_path, err) => Belt.Result.Error(err) |> Js.Promise.resolve
         | AlreadyExists(dist)
         | Ok(dist) =>
           DenSeed.Packs.JsInterop.Output.make(~packs=Array.of_list(configs))
           |> ConfWrite.writeJsonConfig(Node.Path.join2(dist, "data.json"))
           |> Js.Promise.then_(() =>
                Belt.Result.Ok(configs) |> Js.Promise.resolve
              )
         }
       }
     );

type cmdHooks =
  Belt.Result.t(option(DenSeed.Packs.Pack.Hooks.t), DenSeed.Err.t);
let getCmdHooks = (pack: string): Js.Promise.t(cmdHooks) =>
  getCmdConfig(packsPath, pack)
  |> Js.Promise.then_((result: cmdConfig) =>
       result
       ->Belt.Result.map(c =>
           c
           ->DenSeed.Packs.Json.hooksGet
           ->Belt.Option.map(DenSeed.Packs.Pack.Hooks.make)
         )
       ->Js.Promise.resolve
     );
