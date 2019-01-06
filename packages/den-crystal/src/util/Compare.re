type comparison =
  | Eq
  | Lt
  | Gt;

let ab =
  fun
  | (a, b) when a > b => Gt
  | (a, b) when a < b => Lt
  | _ => Eq;

let string_of_comparison =
  fun
  | Gt => "gt"
  | Lt => "lt"
  | Eq => "eq";
