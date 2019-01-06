open Bootstrap;

include Command.Sync.Make({
  type t;
  let command = "ls";
  let description = "List installed packs";

  let listPackages = (): unit =>
    switch (ConfRead.User.getPacks()) {
    | Error(err) => Log.error("Could not read config", err)
    | Ok(packs) =>
      switch (packs) {
      | [||] => Log.info("No installed packs", "")
      | xs => Log.info("Installed packs", xs)
      }
    };

  let handler = _argv => listPackages();
  let builder = yargs => yargs;
});
