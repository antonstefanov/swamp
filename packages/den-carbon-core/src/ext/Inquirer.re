module Questions = {
  [@bs.deriving abstract]
  type _question('a) = {
    [@bs.as "name"]
    arg: string,
    [@bs.as "message"]
    desc: string,
    [@bs.optional]
    choices: array('a),
    [@bs.optional]
    default: 'a,
    [@bs.as "type"]
    type_: string,
  };

  type question;
  type t = array(question);
  let make: unit => t = () => [%raw "[]"];

  [@bs.send] external _push: (t, _question('a)) => unit = "push";
  let push = (arr: t, item: _question('a)) => {
    _push(arr, item);
    arr;
  };

  [@bs.module "inquirer"] [@bs.new]
  external separator: unit => string = "Separator";
};

module Prompt = {
  type t('a) = Js.Promise.t('a);
  [@bs.module "inquirer"] external make: Questions.t => t('a) = "prompt";
};
