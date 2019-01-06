[@bs.module "path"] external _extname: string => string = "extname";
[@bs.module "fs"] external _mkdirSync: string => unit = "mkdirSync";
[@bs.module "fs"]
external _writeFileSync: (string, string) => unit = "writeFileSync";
[@bs.module "fs"]
external _writeFile:
  (string, string, Js.Null_undefined.t(Js.Exn.t) => unit) => unit =
  "writeFile";
[@bs.module "fs"]
external _copyFileSync: (string, string) => unit = "copyFileSync";
[@bs.module "fs"]
external _copyFile:
  (string, string, Js.Null_undefined.t(Js.Exn.t) => unit) => unit =
  "copyFile";
[@bs.module "fs"]
external _unlink: (string, Js.Null_undefined.t(Js.Exn.t) => unit) => unit =
  "unlink";
[@bs.module "fs"]
external _unlink: (string, Js.Null_undefined.t(Js.Exn.t) => unit) => unit =
  "unlink";
[@bs.module "fs"]
external _readdir:
  (string, (Js.Null_undefined.t(Js.Exn.t), array(string)) => unit) => unit =
  "readdir";

module Stats = {
  type t;
  [@bs.module "fs"]
  external _lstat:
    (string, (Js.Null_undefined.t(Js.Exn.t), t) => unit) => unit =
    "lstat";
  [@bs.send] external isFile: t => bool = "";
  [@bs.send] external isDirectory: t => bool = "";
  [@bs.send] external isSymbolicLink: t => bool = "";
  [@bs.send] external isBlockDevice: t => bool = "";
  [@bs.send] external isCharacterDevice: t => bool = "";
  [@bs.send] external isFIFO: t => bool = "";
  [@bs.send] external isSocket: t => bool = "";
  [@bs.get] external mtime: t => Js.Date.t = "";
  [@bs.get] external mtimeMs: t => float = "";
  [@bs.get] external birthtime: t => Js.Date.t = "";
  [@bs.get] external birthtimeMs: t => float = "";
};

let extname = filename =>
  switch (_extname(filename)) {
  | "" => None
  | ext => Some(ext)
  };

let exists = path => Node.Fs.existsSync(path);

module WriteResult = {
  type t =
    | Ok(string)
    | AlreadyExists(string)
    | ErrorNoRights(string, (string, option(string)))
    | Error(string, (string, option(string)));

  let exists =
    fun
    | Ok(_)
    | AlreadyExists(_) => true
    | Error(_)
    | ErrorNoRights(_) => false;

  let ofJsError = (path: string, err): t =>
    switch (Err.code(err)) {
    | Some(code) when code === "EEXIST" => AlreadyExists(path)
    | Some(code) when code === "ENOENT" =>
      ErrorNoRights(path, Err.make(err))
    | Some(code) =>
      Error(path, Err.make(~default={j|"$code : Unknown error"|j}, err))
    | None => Error(path, Err.make(err))
    };
};

let dirname: string = Belt.Option.getWithDefault([%bs.node __dirname], ".");
let swampPath = path => Node.Path.join([|dirname, "../../../", path|]);

[@bs.val] [@bs.return undefined_to_opt] [@bs.scope "JSON"]
external _stringifyAny:
  (
    'a,
    Js.Null_undefined.t((string, string) => Js.Null_undefined.t(string)),
    Js.Null_undefined.t(string)
  ) =>
  option(string) =
  "stringify";
let stringifyAny = (~replacer=?, ~space=?, content) =>
  _stringifyAny(
    content,
    replacer->Belt.Option.map((r, key, value) =>
      r(key, value) |> Js.Null_undefined.fromOption
    )
    |> Js.Null_undefined.fromOption,
    space->Js.Null_undefined.fromOption,
  );

let stringify = a => stringifyAny(~space="  ", a);

let writeFileAsync = (path: string, content: string) =>
  _writeFile(path, content)
  |> Promisify.make
  |> Js.Promise.then_((result: Promisify.t(unit)) =>
       switch (result) {
       | Ok () => WriteResult.Ok(path) |> Js.Promise.resolve
       | Error(err) => WriteResult.ofJsError(path, err) |> Js.Promise.resolve
       }
     );

let writeJsonAsync = (path: string, content: 'a) =>
  writeFileAsync(path, stringify(content)->Belt.Option.getWithDefault(""));

let mkdirSync = path => {
  let mkdir = (path): WriteResult.t => {
    let error = ref(None);
    try (_mkdirSync(path)) {
    | Js.Exn.Error(err) => error := Some(WriteResult.ofJsError(path, err))
    | _ => error := Some(Error(path, ("Non JS error", None)))
    };
    Belt.Option.getWithDefault(error^, Ok(path));
  };
  let rec aux = (paths: list(string)): WriteResult.t =>
    switch (paths) {
    | [] => Error(path, ("No paths to create", None))
    | [path] => mkdir(path)
    | [x, ...xs] =>
      switch (mkdir(x)) {
      | Ok(_)
      | AlreadyExists(_) => aux(xs)
      | ErrorNoRights(path, err)
      | Error(path, err) => Error(path, err)
      }
    };
  /* create a list of folders to create based on path */
  Js.String.split("/", path)
  |> Array.fold_left(
       (acc, pathname) =>
         switch (pathname) {
         | "" => acc
         | p =>
           switch (acc) {
           | [prev, ..._rest] => [Node.Path.join2(prev, p), ...acc]
           | [] =>
             DenSeed.Str.startsWithChar(path, '/') ?
               [Node.Path.join2("/", p)] : [p]
           }
         },
       [],
     )
  |> List.rev
  |> aux;
};

let copyFileAsync = (src: string, dest: string) =>
  _copyFile(src, dest)
  |> Promisify.make
  |> Js.Promise.then_((result: Promisify.t(unit)) =>
       switch (result) {
       | Ok () => WriteResult.Ok(dest) |> Js.Promise.resolve
       | Error(err) => WriteResult.ofJsError(dest, err) |> Js.Promise.resolve
       }
     );

module FsResult = {
  type t('a) = AsyncResult.t('a);

  /* type result('a, 'b) = Promisify.t(t('a, 'b)); */

  let ofJsError = (err: Js.Exn.t) => Belt.Result.Error(Err.make(err));

  let make =
      (fn, arg: 'a)
      : Js.Promise.t(Belt.Result.t('a, (string, option(string)))) =>
    fn(arg)
    |> Promisify.make
    |> Js.Promise.then_(result =>
         (
           switch (AsyncResult.ofJs(result)) {
           | Ok () => Belt.Result.Ok(arg)
           | Error(err) => Belt.Result.Error(err)
           }
         )
         |> Js.Promise.resolve
       );

  let make2 = (fn, arg): Js.Promise.t(t('a)) =>
    fn(arg) |> Promisify.make2 |> Js.Promise.then_(AsyncResult.resolveOfJs);
};

let readSync = path =>
  try (Belt.Result.Ok(Node.Fs.readFileAsUtf8Sync(path))) {
  | Js.Exn.Error(err) => FsResult.ofJsError(err)
  | _ => Error(("Non JS error", None))
  };

module Json = {
  type t = Js.Json.t;

  let parse =
      (contents): Belt.Result.t(Js.Json.t, (string, option(string))) =>
    try (Ok(Js.Json.parseExn(contents))) {
    | Js.Exn.Error(err) => Error(Err.make(err))
    | _ => Error(("Non JS error", None))
    };

  let read = (path): FsResult.t(Js.Json.t) =>
    switch (readSync(path)) {
    | Ok(contents) =>
      switch (parse(contents)) {
      | Ok(json) => Ok(json)
      | Error(err) => Error(err)
      }
    | Error(err) => Error(err)
    };
};

let deleteFileAsync = (path: string) => FsResult.make(_unlink, path);

let lstat = (path: string) => FsResult.make2(Stats._lstat, path);

let readDir = (path: string) => FsResult.make2(_readdir, path);

module FileResult = {
  type t =
    | File(string)
    | Dir(string)
    | Other(string);

  let ofStat = (filename, stat) =>
    if (Stats.isFile(stat)) {
      File(filename);
    } else if (Stats.isDirectory(stat)) {
      Dir(filename);
    } else {
      Other(filename);
    };

  let isDir =
    fun
    | Other(_name)
    | File(_name) => false
    | Dir(_name) => true;

  let isFile =
    fun
    | Other(_name)
    | Dir(_name) => false
    | File(_name) => true;

  let unwrap =
    fun
    | Other(name)
    | Dir(name)
    | File(name) => name;

  let getFilename =
    fun
    | Other(_name)
    | Dir(_name) => None
    | File(name) => Some(name);

  let isImg = name =>
    switch (extname(name)) {
    | None => false
    | Some(ext) =>
      switch (ext) {
      | ".jpg"
      | ".jpeg"
      | ".png"
      | ".svg"
      | ".gif" => true
      | _ => false
      }
    };

  let rec allOrNone =
          (unwrapped: list('a), fileResults: list(FsResult.t('a))) =>
    switch (fileResults) {
    | [] => Belt.Result.Ok(unwrapped |> List.rev)
    | [x, ...xs] =>
      switch (x) {
      | Ok(fileResult) => allOrNone([fileResult, ...unwrapped], xs)
      | Error(error) => Error(error)
      }
    };

  let make = (path: string, filename: string) =>
    Node.Path.join2(path, filename)
    |> lstat
    |> Js.Promise.then_((result: FsResult.t(Stats.t)) =>
         switch (result) {
         | Ok(stat) =>
           Belt.Result.Ok(ofStat(filename, stat)) |> Js.Promise.resolve
         | Error(err) => Belt.Result.Error(err) |> Js.Promise.resolve
         }
       );

  type readDirResult = FsResult.t(list(t));
  let readDir = (path): Js.Promise.t(readDirResult) =>
    readDir(path)
    |> Js.Promise.then_((result: FsResult.t(array(string))) =>
         switch (result) {
         | Ok(files) =>
           files
           |> Array.map(file => make(path, file))
           |> Js.Promise.all
           |> Js.Promise.then_(result =>
                result |> Array.to_list |> allOrNone([]) |> Js.Promise.resolve
              )
         | Error(err) => Belt.Result.Error(err) |> Js.Promise.resolve
         }
       );

  type readDirUnwrappedResult = AsyncResult.t(list(string));
  let readDirUnwrapped = (filter, path): Js.Promise.t(readDirUnwrappedResult) =>
    readDir(path)
    |> Js.Promise.then_((result: readDirResult) =>
         switch (result) {
         | Ok(files) =>
           files
           |> List.filter(filter)
           |> List.map(unwrap)
           |> (dirs => Belt.Result.Ok(dirs))
           |> Js.Promise.resolve
         | Error(error) => Belt.Result.Error(error) |> Js.Promise.resolve
         }
       );
};
