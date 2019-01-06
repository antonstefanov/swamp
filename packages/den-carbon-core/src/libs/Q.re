open Inquirer;
open Questions;
open DenSeed;

type t('a) = Prompt.t('a);
let make = Prompt.make;
let empty = Questions.make;

let separator = Questions.separator;

let confirm = (~arg, ~desc, ~default: option(bool)=?, questions: Questions.t) =>
  _question(~arg, ~desc, ~default?, ~type_="confirm", ()) |> push(questions);

let input = (~arg, ~desc, ~default: option('a)=?, questions: Questions.t) =>
  _question(~arg, ~desc, ~default?, ~type_="input", ()) |> push(questions);

let list =
    (
      ~arg,
      ~desc,
      ~default: option('a)=?,
      ~choices: array('a),
      questions: Questions.t,
    ) =>
  _question(~arg, ~desc, ~default?, ~type_="list", ~choices, ())
  |> push(questions);

let rawList =
    (
      ~arg,
      ~desc,
      ~default: option(string)=?,
      ~choices: array(string),
      questions: Questions.t,
    ) =>
  _question(~arg, ~desc, ~default?, ~type_="rawlist", ~choices, ())
  |> push(questions);

module Ask = {
  module Confirm = {
    type t = Prompt.t(bool);

    module Result = {
      [@bs.deriving abstract]
      type t = {confirm: bool};
    };

    let make = (~desc="Please confirm", default): t =>
      empty()
      |> confirm(~arg="confirm", ~desc, ~default)
      |> make
      |> Js.Promise.then_((result: Result.t) =>
           Js.Promise.resolve(result->Result.confirmGet)
         );
  };

  module DoubleConfirm = {
    type t = Prompt.t(bool);

    module Result = {
      [@bs.deriving abstract]
      type t = {confirm: string};
    };

    let getConfirmText = confirmation =>
      Chalk.bold("Enter " ++ Chalk.cyan(confirmation) ++ " to confirm");

    let make = (~desc=?, confirmation: string): t =>
      empty()
      |> input(
           ~arg="confirm",
           ~desc=
             Belt.Option.getWithDefault(desc, "")
             ++ (confirmation |> getConfirmText),
         )
      |> make
      |> Js.Promise.then_((result: Result.t) =>
           Js.Promise.resolve(result->Result.confirmGet === confirmation)
         );
  };

  module Input = {
    type t = Prompt.t(string);

    module Result = {
      [@bs.deriving abstract]
      type t = {input: string};
    };

    let make = (~desc: string, ~default: option(string)=?, ()): t =>
      empty()
      |> input(~arg="input", ~desc, ~default?)
      |> make
      |> Js.Promise.then_((result: Result.t) =>
           Js.Promise.resolve(result->Result.inputGet)
         );
  };
};

module Decorator = {
  let withConfirm =
      (
        ~confirmDefault=false,
        ~confirmOverride,
        ~confirmText,
        ~path,
        ~confirm,
        ~cancel,
      ) =>
    switch (confirmOverride, Fs.exists(path)) {
    | (false, _) => confirm()
    | (true, false) => confirm()
    | (true, true) =>
      Ask.Confirm.make(confirmDefault, ~desc=confirmText(path))
      |> Js.Promise.then_(override => override ? confirm() : cancel())
    };

  let withDoubleConfirm =
      (
        ~confirmation: string,
        ~confirmOverride,
        ~confirmText,
        ~path,
        ~confirm,
        ~cancel,
      ) =>
    switch (confirmOverride) {
    | false => confirm()
    | true =>
      Ask.DoubleConfirm.make(confirmation, ~desc=confirmText(path))
      |> Js.Promise.then_(override => override ? confirm() : cancel())
    };
};
