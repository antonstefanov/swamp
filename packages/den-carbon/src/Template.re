open Bootstrap;

include Command.Sync.Make({
  type t;

  let command = "ask <question> [answers..]";
  let description = "Ask about the world, the universe or anything else";

  let handler = _argv => ();

  let builder = yargs =>
    Args.(yargs |> Arg.String.make(~arg="arg_name", ~desc="Description"));
});
