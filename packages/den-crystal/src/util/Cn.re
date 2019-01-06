let make = (classNames: list(string)): string =>
  classNames
  |> List.filter(className => className !== "")
  |> String.concat(" ");

let ifTrue = (className: string, maybe: bool) => maybe ? className : "";

let ifSome = (className, maybe) =>
  switch (maybe) {
  | Some(_) => className
  | None => ""
  };

let mapSome = (map, maybe) =>
  switch (maybe) {
  | Some(value) => value |> map
  | None => ""
  };

let prependIfSome = (prepend, maybe) =>
  switch (maybe) {
  | Some(value) => prepend ++ value
  | None => ""
  };

let appendIfSome = (append, maybe) =>
  switch (maybe) {
  | Some(value) => value ++ append
  | None => ""
  };

let unwrap = maybe =>
  switch (maybe) {
  | Some(value) => value
  | None => ""
  };
