let some = x => Some(x);

let map = (fn, maybe) =>
  switch (maybe) {
  | Some(x) => fn(x) |> some
  | None => None
  };

let mapFlat = (fn, maybe) =>
  switch (maybe) {
  | Some(x) => fn(x)
  | None => None
  };

let mapIgnore = (fn, maybe) =>
  switch (maybe) {
  | Some(x) =>
    fn(x);
    ();
  | None => ()
  };

let run = (fn, arg) =>
  switch (fn) {
  | Some(f) => f(arg)
  | None => ()
  };

let withDefault = (defaultValue, maybe) =>
  switch (maybe) {
  | Some(x) => x
  | None => defaultValue
  };

let trueIfSome = maybe =>
  switch (maybe) {
  | Some(_) => true
  | None => false
  };
