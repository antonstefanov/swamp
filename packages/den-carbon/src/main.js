"use strict";
let yargs;
try {
  yargs = require("yargs");
} catch (e) {
  console.log(
    "Could not load yargs, make sure to run 'yarn' before starting",
    e,
  );
  return;
}
let logo;
try {
  logo = require("./JsImport.bs").logo;
} catch (e) {
  console.log(
    "Could not load Logo.bs, make sure to run 'yarn init-all' before running commands",
    e,
  );
  return;
}
// try {
//   require("den-packs-core");
// } catch (e) {
//   console.log(
//     "Main command packs could not be loaded, try re-adding carbon or installing its packages",
//     e,
//   );
//   return;
// }
// const denPacksCorePath = require.resolve("den-packs-core");
const {
  includeOnlyCmd,
  log,
  config: { get: getConfig },
} = require("./JsImport.bs");
const pathJoin = require("path").join;

const config = getConfig();
const arg = index => process.argv[index + 2];
const AUTOCOMPLETE_ARG = "completion";
const options = {
  include: includeOnlyCmd,
};

(async () => {
  if (arg(0) !== AUTOCOMPLETE_ARG && config.showLogo) {
    log.info(logo, "");
  }
  try {
    yargs.commandDir("../node_modules/den-packs-core/src", options);
    try {
      config.packs &&
        config.packs.map(path =>
          yargs.commandDir(pathJoin(config.packsPath, path), options),
        );
    } catch (err) {
      if (err.code === "ENOENT") {
        log.warn("Packages dir does not exist", err.message);
      } else {
        log.error("Error during packs parsing", err);
      }
    }
    try {
      config.cmdDirs &&
        config.cmdDirs.map(path => yargs.commandDir(path, options));
    } catch (err) {
      if (err.code === "ENOENT") {
        log.warn("Commands dir does not exist", err.message);
      } else {
        log.error("Error during user commands parsing", err);
      }
    }
    yargs
      .demandCommand()
      .completion(AUTOCOMPLETE_ARG)
      .recommendCommands()
      .help()
      .alias("help", "h").argv;
  } catch (err) {
    log.error("Unhandled Error", err);
  }
})();
