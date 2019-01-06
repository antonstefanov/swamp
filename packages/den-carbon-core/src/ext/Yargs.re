type t;

type arg_type =
  | Bool
  | Number
  | String
  | Array;
let string_of_arg_type =
  fun
  | Bool => "boolean"
  | Number => "number"
  | String => "string"
  | Array => "array";

[@bs.deriving abstract]
type positionalOptions('a, 'b) = {
  [@bs.optional]
  alias: string,
  [@bs.optional]
  choices: array('a),
  [@bs.optional]
  default: 'a,
  [@bs.optional] [@bs.as "default"]
  defaultArray: array('a),
  [@bs.optional]
  desc: string,
  [@bs.optional] [@bs.as "type"]
  type_: string,
  [@bs.optional]
  coerce: 'b => 'a,
};

let positionalOptions =
    (
      ~alias=?,
      ~choices=?,
      ~default=?,
      ~defaultArray=?,
      ~desc=?,
      ~coerce=?,
      ~type_: option(arg_type)=?,
      (),
    ) =>
  positionalOptions(
    ~alias?,
    ~choices?,
    ~default?,
    ~defaultArray?,
    ~desc?,
    ~coerce?,
    ~type_=?Belt.Option.map(type_, string_of_arg_type),
    (),
  );

[@bs.send]
external positional: (t, string, positionalOptions('a, 'b)) => t = "";

[@bs.deriving abstract]
type jsArg('a, 'b) = {
  [@bs.optional]
  alias: string,
  [@bs.optional]
  default: 'a,
  [@bs.optional]
  desc: string,
  [@bs.optional] [@bs.as "type"]
  type_: string,
  [@bs.optional]
  choices: array('b),
  [@bs.optional]
  required: bool,
  [@bs.optional]
  requiresArg: bool,
  [@bs.optional]
  coerce: 'b => 'a,
};
[@bs.send] external cmdOption: (t, string, jsArg('a, 'b)) => t = "option";

let cmdArg =
    (
      ~alias=?,
      ~default=?,
      ~desc=?,
      ~type_: option(arg_type)=?,
      ~choices=?,
      ~coerce=?,
      ~required=false,
      ~requiresArg=false,
      (),
    ) =>
  jsArg(
    ~alias?,
    ~default?,
    ~desc?,
    ~choices?,
    ~coerce?,
    ~type_=?Belt.Option.map(type_, string_of_arg_type),
    ~required,
    ~requiresArg,
    (),
  );

[@bs.send] external help: t => t = "";

[@bs.send] external alias: (t, string, string) => t = "";

[@bs.deriving abstract]
type dirOptions = {
  [@bs.optional]
  exclude: Js.Re.t,
};

[@bs.send] external commandDir: (t, string, dirOptions) => t = "commandDir";
