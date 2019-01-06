open Bootstrap;

include Command.Sync.Make({
  type t;
  let command = "ls";
  let description = "List installed packs";

  let listPackages = (): unit =>
    switch (ConfRead.User.getPacks()) {
    | Error(err) => Log.error("Could not read config", err)
    | Ok(packs) => packs |> Log.info("Installed packs")
    };

  let handler = _argv => listPackages();
  let builder = yargs => yargs;
});
