open Yargs;

module Arg = {
  type required('a) =
    | Required
    | Optional
    | Default('a);

  let bool_of_required =
    fun
    | Required => true
    | Optional => false
    | Default(_) => false;

  module Base = {
    let unbox_default =
      fun
      | Required => None
      | Optional => None
      | Default(default) => Some(default);

    type choices('a, 'b) =
      | Array(array('a))
      | Coerce(array('b), 'b => 'a);

    let unbox_choices =
      fun
      | Array(xs)
      | Coerce(xs, _) => Some(xs);

    let unbox_coerce =
      fun
      | Array(_) => None
      | Coerce(_, c) => Some(c);
  };

  module String = {
    include Base;

    let make =
        (
          ~alias=?,
          ~required=Required,
          ~choices: option(choices(string, 'b))=?,
          ~desc,
          ~arg,
          yargs,
        ) =>
      cmdOption(
        yargs,
        arg,
        cmdArg(
          ~alias?,
          ~default=?unbox_default(required),
          ~desc,
          ~type_=String,
          ~choices=?Belt.Option.flatMap(choices, unbox_choices),
          ~coerce=?Belt.Option.flatMap(choices, unbox_coerce),
          ~required=bool_of_required(required),
          ~requiresArg=true,
          (),
        ),
      );
  };

  module Int = {
    include Base;

    let make =
        (
          ~alias=?,
          ~required=Required,
          ~choices: option(choices(int, 'b))=?,
          ~desc,
          ~arg,
          yargs,
        ) =>
      cmdOption(
        yargs,
        arg,
        cmdArg(
          ~alias?,
          ~default=?unbox_default(required),
          ~desc,
          ~type_=Number,
          ~choices=?Belt.Option.flatMap(choices, unbox_choices),
          ~coerce=?Belt.Option.flatMap(choices, unbox_coerce),
          ~required=bool_of_required(required),
          ~requiresArg=true,
          (),
        ),
      );
  };

  module Float = {
    include Base;

    let make =
        (
          ~alias=?,
          ~required=Required,
          ~choices: option(choices(float, 'b))=?,
          ~desc,
          ~arg,
          yargs,
        ) =>
      cmdOption(
        yargs,
        arg,
        cmdArg(
          ~alias?,
          ~default=?unbox_default(required),
          ~desc,
          ~type_=Number,
          ~choices=?Belt.Option.flatMap(choices, unbox_choices),
          ~coerce=?Belt.Option.flatMap(choices, unbox_coerce),
          ~required=bool_of_required(required),
          ~requiresArg=true,
          (),
        ),
      );
  };

  module Enum = {
    include Base;
    type type_('a) =
      | Int(required(int), array(int), int => 'a)
      | Float(required(float), array(float), float => 'a)
      | String(required(string), array(string), string => 'a);

    external jsv: 'a => string = "%identity";
    let jsDefault =
      fun
      | Int(req, _, _) => req->unbox_default->Belt.Option.map(jsv)
      | Float(req, _, _) => req->unbox_default->Belt.Option.map(jsv)
      | String(req, _, _) => req->unbox_default->Belt.Option.map(jsv);
    let jsRequired =
      fun
      | Int(req, _, _) => req->bool_of_required
      | Float(req, _, _) => req->bool_of_required
      | String(req, _, _) => req->bool_of_required;

    /* this pretends that all conversions are string->string to satisfy the typechecker */
    external jsChoices: array('a) => array(string) = "%identity";
    let choices =
      fun
      | Int(_, xs, _) => jsChoices(xs)
      | Float(_, xs, _) => jsChoices(xs)
      | String(_, xs, _) => jsChoices(xs);

    /* this pretends that all conversions are string->string to satisfy the typechecker */
    type jsCoerce = string => string;
    external jsCoerce: ('a => 'b) => jsCoerce = "%identity";

    let coerce =
      fun
      | Int(_, _, c) => jsCoerce(c)
      | Float(_, _, c) => jsCoerce(c)
      | String(_, _, c) => jsCoerce(c);

    let type_of_type = (type_: type_('a)): arg_type =>
      switch (type_) {
      | Int(_, _, _) => Number
      | Float(_, _, _) => Number
      | String(_, _, _) => String
      };

    let make = (~alias=?, ~type_, ~desc, ~arg, yargs) =>
      cmdOption(
        yargs,
        arg,
        cmdArg(
          ~alias?,
          ~default=?jsDefault(type_),
          ~desc,
          ~type_=type_of_type(type_),
          ~choices=choices(type_),
          ~coerce=coerce(type_),
          ~required=jsRequired(type_),
          ~requiresArg=true,
          (),
        ),
      );
  };

  module Bool = {
    include Base;

    let make = (~alias=?, ~required=Default(false), ~desc, ~arg, yargs) =>
      cmdOption(
        yargs,
        arg,
        cmdArg(
          ~alias?,
          ~default=?unbox_default(required),
          ~desc,
          ~type_=Bool,
          ~required=bool_of_required(required),
          ~requiresArg=false,
          (),
        ),
      );
  };

  module Array = {
    include Base;

    let make =
        (
          ~alias=?,
          ~required: required(array('a))=Required,
          ~choices: option(choices(array(string), 'b))=?,
          ~desc,
          ~arg,
          yargs,
        ) =>
      cmdOption(
        yargs,
        arg,
        cmdArg(
          ~alias?,
          ~default=?unbox_default(required),
          ~desc,
          ~type_=Array,
          ~choices=?Belt.Option.flatMap(choices, unbox_choices),
          ~coerce=?Belt.Option.flatMap(choices, unbox_coerce),
          ~required=bool_of_required(required),
          ~requiresArg=true,
          (),
        ),
      );
  };
};

module Pos = {
  module Base = {
    type choices('a, 'b) =
      | Array(array('a))
      | Coerce(array('b), 'b => 'a);

    /* this pretends that all conversions are string->string to satisfy the typechecker */
    /* external jsChoices: array('a) => array(string) = "%identity"; */
    let arr_of_choices =
      fun
      | Array(xs)
      | Coerce(xs, _) => xs;

    let unbox_coerce =
      fun
      | Array(_) => None
      | Coerce(_, c) => Some(c);
  };

  module String = {
    include Base;

    let make =
        (
          ~alias=?,
          ~choices: option(choices(string, 'b))=?,
          ~default=?,
          ~desc=?,
          ~type_: option(arg_type)=?,
          ~arg,
          yargs,
        ) =>
      positional(
        yargs,
        arg,
        positionalOptions(
          ~alias?,
          ~choices=?Belt.Option.map(choices, arr_of_choices),
          ~coerce=?Belt.Option.flatMap(choices, unbox_coerce),
          ~default?,
          ~desc?,
          ~type_?,
          (),
        ),
      );
  };

  module Int = {
    include Base;
    let make =
        (
          ~alias=?,
          ~choices: option(choices(int, 'b))=?,
          ~default=?,
          ~desc=?,
          ~type_: option(arg_type)=?,
          ~arg,
          yargs,
        ) =>
      positional(
        yargs,
        arg,
        positionalOptions(
          ~alias?,
          ~choices=?Belt.Option.map(choices, arr_of_choices),
          ~coerce=?Belt.Option.flatMap(choices, unbox_coerce),
          ~default?,
          ~desc?,
          ~type_?,
          (),
        ),
      );
  };

  module Float = {
    include Base;
    let make =
        (
          ~alias=?,
          ~choices: option(choices(float, 'b))=?,
          ~default=?,
          ~desc=?,
          ~type_: option(arg_type)=?,
          ~arg,
          yargs,
        ) =>
      positional(
        yargs,
        arg,
        positionalOptions(
          ~alias?,
          ~choices=?Belt.Option.map(choices, arr_of_choices),
          ~coerce=?Belt.Option.flatMap(choices, unbox_coerce),
          ~default?,
          ~desc?,
          ~type_?,
          (),
        ),
      );
  };

  module Array = {
    include Base;
    let make =
        (
          ~alias=?,
          ~choices: option(choices(array('a), 'b))=?,
          ~default=?,
          ~desc=?,
          ~arg,
          yargs,
        ) =>
      positional(
        yargs,
        arg,
        positionalOptions(
          ~alias?,
          ~choices=?Belt.Option.map(choices, arr_of_choices),
          ~coerce=?Belt.Option.flatMap(choices, unbox_coerce),
          ~default?,
          ~desc?,
          ~type_=Array,
          (),
        ),
      );
  };
};

let interactiveArg = (~default, yargs) =>
  Arg.Bool.make(
    yargs,
    ~arg="interactive",
    ~alias="i",
    ~desc="Interactive mode",
    ~required=Default(default),
  );

let help = yargs => yargs->help->alias("help", "h");

let transform_completion = (fn, current, argv) =>
  fn(current, argv) |> Array.of_list;
let completion = (fn: (string, 'a) => list(string), yargs: t) =>
  Yargs.completion(yargs, "completion", transform_completion(fn));

let dirOptions = dirOptions;
/* cannot call it from a fn, because dir is relative to caller path */
[@bs.send]
external dir: (Yargs.t, string, dirOptions) => Yargs.t = "commandDir";

let excludeHelpers =
  {|/\/[0-9a-zA-Z]*(Shell|Helper|Q)\.(jsx?|tsx?)$/i|} |> Js.Re.fromString;

let includeOnlyCmd = [%re {|/.*(Cmd|Pack)\.(bs\.)?.{1,5}$/i|}];
