module CacheAsync = {
  type memoizeAsyncResult('a) =
    | Loading(Js.Promise.t('a))
    | Result('a);

  type t('a) = string => Js.Promise.t('a);
  let exec = (~cache, ~fn, arg) => {
    (cache^)->Belt.Map.String.has(arg) ?
      (cache^)->Belt.Map.String.getExn(arg) |> Js.Promise.resolve :
      {
        let promise = fn(arg);
        cache := (cache^)->Belt.Map.String.set(arg, Loading(promise));
        promise
        |> Js.Promise.then_(r => {
             let result = Result(r);
             cache := (cache^)->Belt.Map.String.set(arg, result);
             result |> Js.Promise.resolve;
           });
      };
  };
  let make =
      (
        ~cache: option(ref(Belt.Map.String.t(memoizeAsyncResult('a))))=?,
        fn: t('a),
      )
      : t('a) => {
    let cache =
      Belt.Option.getWithDefault(cache, ref(Belt.Map.String.empty));
    /* in reason promises are not auto-chained */
    arg =>
      exec(~cache, ~fn, arg)
      |> Js.Promise.then_(
           fun
           | Loading(promise) => promise
           | Result(result) => result |> Js.Promise.resolve,
         );
  };
};

module CacheSync = {
  type t('a) = string => 'a;
  let make = (~cache=ref(Belt.Map.String.empty), fn, arg) => {
    (cache^)->Belt.Map.String.has(arg) ?
      (cache^)->Belt.Map.String.getExn(arg) :
      {
        let result = fn(arg);
        cache := (cache^)->Belt.Map.String.set(arg, result);
        result;
      };
  };
};
