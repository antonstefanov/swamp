module Config = {
  open DenSeed;

  [@bs.deriving abstract]
  type _config = {
    logLevel: LogLevel.level,
    cmdDirs: array(string),
    showLogo: bool,
    editor: string,
    packsPath: string,
    packs: array(string),
  };
  type t = _config;

  module User = {
    [@bs.deriving abstract]
    type t = {
      [@bs.optional]
      mutable logLevel: LogLevel.level,
      [@bs.optional]
      mutable cmdDirs: array(string),
      [@bs.optional]
      mutable showLogo: bool,
      [@bs.optional]
      mutable editor: string,
      [@bs.optional]
      mutable packsPath: string,
      [@bs.optional]
      mutable packs: array(string),
    };
    let make = t;
  };

  let dirname: string = Belt.Option.getWithDefault([%bs.node __dirname], ".");
  let src = path => Node.Path.join([|dirname, ".", path|]);

  let make =
      (
        ~logLevel=LogLevel.Info,
        ~cmdDirs=[||],
        ~showLogo=true,
        ~editor="vi",
        ~packsPath="/work/carbon-packs/src/packs",
        ~packs=[|"web"|],
        (),
      ) =>
    _config(~logLevel, ~cmdDirs, ~showLogo, ~editor, ~packsPath, ~packs);

  module Path = {
    module Dir = {
      let swamp = "/usr/local/etc/swamp";
      let carbon = Node.Path.join2(swamp, "carbon");
      let crystal = Node.Path.join2(swamp, "crystal");
    };
    let json = Node.Path.join2(Dir.carbon, "config.json");
    let userJson = Node.Path.join2(Dir.carbon, "user-config.json");
    let js = Node.Path.join2(Dir.carbon, "index.js");
    let defaultConfigReader = "./den-carbon-core/src/conf/defaultConfigReader.js";
  };

  type export = {. "default": (. unit) => t};
  /* require a file */
  [@bs.val] external require: string => export = "require";
  let fallbackGetMerger = require(Path.defaultConfigReader);
  let jspath = Path.js;
  let requireDefault = () =>
    try (require(jspath)) {
    | e =>
      switch (e->Js.Exn.asJsExn) {
      | None => e |> raise
      | Some(jserr) =>
        jserr->Js.Exn.message->Belt.Option.getExn
        === "Dynamic requires are not currently supported by rollup-plugin-commonjs" ?
          {
            Js.log2(
              Chalk.yellow("Dynamic requires are not supported yet"),
              "Using default merger.",
              /* rollup does not support dynamic require -> use fallback */
            );
            fallbackGetMerger;
          } :
          {
            e |> raise;
          }
      }
    };
  let default = requireDefault()##default;
  let getJsDefault = () => {
    /* try to access, so that it fails immediately, instead of lazily */
    default(.) |> ignore;
    default;
  };
  let get = () => {
    let fn =
      try (getJsDefault()) {
      | e =>
        switch (e->Js.Exn.asJsExn) {
        | None => e->raise
        | Some(_err) =>
          Js.log2(
            Chalk.redBright("Could not open config file. ")
            ++ Chalk.cyan(
                 "Run 'o conf init' to create a default config file.",
               ),
            "Using default config temporarily",
          );
          ((.) => make());
        }
      };
    fn(.);
  };
};
