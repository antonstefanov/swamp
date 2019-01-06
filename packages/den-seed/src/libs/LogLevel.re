type level =
  | Trace
  | Debug
  | Info
  | Success
  | Warn
  | Error
  | Silent;

let levels = [Trace, Debug, Info, Success, Warn, Error, Silent];

let int_of_level =
  fun
  | Trace => 0
  | Debug => 1
  | Info => 2
  | Warn => 3
  | Success => 4
  | Error => 5
  | Silent => 6;

let level_of_int =
  fun
  | 0 => Trace
  | 1 => Debug
  | 2 => Info
  | 3 => Warn
  | 4 => Success
  | 5 => Error
  | 6 => Silent
  | l => failwith("no such log level " ++ string_of_int(l));

let string_of_level =
  fun
  | Trace => "trace"
  | Debug => "debug"
  | Info => "info"
  | Warn => "warn"
  | Success => "success"
  | Error => "error"
  | Silent => "silent";

let level_of_string =
  fun
  | "trace" => Trace
  | "debug" => Debug
  | "info" => Info
  | "warn" => Warn
  | "success" => Success
  | "error" => Error
  | "silent" => Silent
  | l => failwith("no such log level " ++ l);

let aboveLevel = (baseLevel, level) =>
  int_of_level(level) <= int_of_level(baseLevel);

let possibleValues =
  levels
  |> List.map(level =>
       "("
       ++ (level |> int_of_level |> string_of_int)
       ++ "-"
       ++ string_of_level(level)
       ++ ")"
     )
  |> Lst.join(", ");
