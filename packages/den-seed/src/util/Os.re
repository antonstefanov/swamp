module Platform = {
  type t =
    | Aix
    | Mac
    | FreeBsd
    | Linux
    | OpenBsd
    | Sunos
    | Windows;

  [@bs.val] external _platform: string = "process.platform";

  let platform_of_string =
    fun
    | "aix" => Aix
    | "darwin" => Mac
    | "freebsd" => FreeBsd
    | "linux" => Linux
    | "openbsd" => OpenBsd
    | "sunos" => Sunos
    | "win32" => Windows
    | p => failwith("unknown platform " ++ p);

  let value = _platform |> platform_of_string;
};

module Bashrc = {
  let location =
    Platform.(
      fun
      | Aix => "~/.bashrc"
      | Mac => "~/.bash_profile"
      | FreeBsd => "~/.bashrc"
      | Linux => "~/.bashrc"
      | OpenBsd => "~/.bashrc"
      | Sunos => "~/.bashrc"
      | Windows => failwith("Windows is not supported")
    );
};
