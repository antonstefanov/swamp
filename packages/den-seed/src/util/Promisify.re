type t('a) = Belt.Result.t('a, Js.Exn.t);

let make = fn: Js.Promise.t(t(unit)) =>
  Js.Promise.make((~resolve, ~reject as _) =>
    fn(error =>
      switch (Js.Null_undefined.toOption(error)) {
      | Some(err) => resolve(. Belt.Result.Error(err))
      | None => resolve(. Ok())
      }
    )
  );

let make2 = fn: Js.Promise.t(t('a)) =>
  Js.Promise.make((~resolve, ~reject as _) =>
    fn((error, result) =>
      switch (Js.Null_undefined.toOption(error)) {
      | Some(err) => resolve(. Belt.Result.Error(err))
      | None => resolve(. Ok(result))
      }
    )
  );
