open Bootstrap;

type output =
  | Console
  | Bash(string);

let autocomplete = (output: output) => {
  let args =
    switch (output) {
    | Console => []
    | Bash(bashLocation) => [">> " ++ bashLocation]
    };
  let args = ["completion", ...args];
  Cmd.(
    runAsync(
      ~cmd="o",
      ~args,
      ~cmdOptions=CmdOptions.make(~shell=true, ~stderrAsProgress=true, ()),
      (),
    )
  );
};
