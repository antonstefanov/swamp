open Chalk;
open LogLevel;

module type T = {
  let trace: (string, 'value) => 'value;
  let generic: (string, 'value) => unit;
  let debug: (string, 'value) => unit;
  let info: (string, 'value) => unit;
  let warn: (string, 'value) => unit;
  let success: (string, 'value) => unit;
  let error: (string, 'value) => unit;
};

module ConsoleLogger: T = {
  /* tuples are arrays -> expand them as args */
  let consoleLog: ('a, 'b) => unit = [%raw
    (message, value) => {|
      if(Array.isArray(value)){
        console.log(message, ...value);
      } else {
        console.log(message, value);
      }
    |}
  ];

  let generic = (message, value) =>
    consoleLog(Obj.magic(message), Obj.magic(value));
  let debug = (message, value) =>
    white(message)->Obj.magic->consoleLog(value->Obj.magic);
  let trace = (message, value) => {
    debug(message->Obj.magic, value->Obj.magic);
    value;
  };
  let info = (message, value) =>
    blue(message)->Obj.magic->consoleLog(value->Obj.magic);
  let warn = (message, value) =>
    yellow(message)->Obj.magic->consoleLog(value->Obj.magic);
  let success = (message, value) =>
    cyan(message)->Obj.magic->consoleLog(value->Obj.magic);
  let error = (message, value) =>
    red(message)->Obj.magic->consoleLog(value->Obj.magic);
};

let noop = (_message, _value) => ();
let getLog = (~level, ~noop=noop, fn, baseLevel) =>
  aboveLevel(baseLevel, level) ? fn : noop;

module type LoggerOptions = {let level: level;};

module type LevelLogger = {include T;};

module LevelLogger = (Logger: T, Options: LoggerOptions) : LevelLogger => {
  let al = l => aboveLevel(l, Options.level);
  let generic = (m, v) => al(Trace) ? Logger.generic(m, v) : noop(m, v);
  let debug = (m, v) => al(Debug) ? Logger.debug(m, v) : noop(m, v);
  let trace = (m, v) => al(Info) ? Logger.trace(m, v) : v;
  let info = (m, v) => al(Info) ? Logger.info(m, v) : noop(m, v);
  let warn = (m, v) => al(Warn) ? Logger.warn(m, v) : noop(m, v);
  let success = (m, v) => al(Success) ? Logger.success(m, v) : noop(m, v);
  let error = (m, v) => al(Error) ? Logger.error(m, v) : noop(m, v);
  /*
    types don't work:
     let gl = getLog(~level=Options.level);
     let generic = gl(Logger.generic, Trace);
     let debug = gl(Logger.debug, Debug);
     let trace =
       getLog(~level=Options.level, ~noop=(m, v) => v, Logger.trace, Debug);
     let info = gl(Logger.info, Info);
     let warn = gl(Logger.warn, Warn);
     let success = gl(Logger.success, Success);
     let error = gl(Logger.error, Error);
   */
};

module JsLogger = {
  type log('value) = (string, 'value) => unit;
  type t = {
    .
    "generic": log(string),
    "debug": log(string),
    "info": log(string),
    "warn": log(string),
    "success": log(string),
    "error": log(string),
  };

  let make: level => t =
    level => {
      let gl = getLog(~level);
      ConsoleLogger.{
        "generic": gl(generic, Trace),
        "debug": gl(debug, Debug),
        "info": gl(info, Info),
        "warn": gl(warn, Warn),
        "success": gl(success, Success),
        "error": gl(error, Error),
      };
    };
};

module TaskLogger = {
  let indexStyler = (current, total) => bold({j|[$current/$total]|j});
  let indexStyle = (total, currentIndex) =>
    indexStyler(currentIndex + 1, total);
  let logTaskStart = taskIndex => Js.log({j|>>>>>>> task start $taskIndex|j});
  let logTaskEnd = taskIndex => Js.log({j|<<<<<<< done $taskIndex|j});

  let log = (tasks: array(Js.Promise.t('a))) => {
    let total = tasks |> Array.length;
    let taskIndex = indexStyle(total);
    Js.log("running " ++ (total |> string_of_int |> bold) ++ " tasks");

    let rec runTask = (i, results) =>
      if (i >= total) {
        Js.Promise.resolve(results);
      } else {
        let ti = taskIndex(i);
        logTaskStart(ti);
        tasks[i]
        |> Js.Promise.then_(result => {
             logTaskEnd(ti);
             runTask(i + 1, [result, ...results]);
           });
      };

    runTask(0, [])
    |> Js.Promise.then_(results => results |> List.rev |> Js.Promise.resolve);
  };
};
