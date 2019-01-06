type accent =
  | Primary
  | Secondary
  | Info
  | Success
  | Warning
  | Danger
  | Light
  | Dark
  | Link
  | Accent;
let string_of_accent =
  fun
  | Primary => "primary"
  | Secondary => "secondary"
  | Info => "info"
  | Success => "success"
  | Warning => "warning"
  | Danger => "danger"
  | Light => "light"
  | Dark => "dark"
  | Link => "link"
  | Accent => "accent";

let bg_of_accent = (~includeColor=false, ~transparent=false, accent) =>
  "bg--"
  ++ Cn.ifTrue("c", includeColor)
  ++ string_of_accent(accent)
  ++ Cn.ifTrue("-t", transparent);

type theme =
  | Light
  | Dark;
let string_of_theme =
  fun
  | Light => "light"
  | Dark => "dark";

type size =
  | Usm
  | Sm
  | Md
  | Lg
  | Xl;
let string_of_size =
  fun
  | Usm => "usm"
  | Sm => "sm"
  | Md => "md"
  | Lg => "lg"
  | Xl => "xl";

type position =
  | Static
  | Relative
  | Absolute
  | Fixed
  | Sticky;
let string_of_position =
  fun
  | Static => "static"
  | Relative => "relative"
  | Absolute => "absolute"
  | Fixed => "fixed"
  | Sticky => "sticky";

let withMeasure = (measure, text) => text ++ measure;

let rem = withMeasure("rem");
let px = withMeasure("px");
let perc = withMeasure("%");

let rem_of_int = value => value |> string_of_int |> rem;
let px_of_int = value => value |> string_of_int |> px;
let perc_of_int = value => value |> string_of_int |> perc;

type unit =
  | Rem
  | Px
  | Percent;

let string_of_unit =
  fun
  | Rem => "rem"
  | Px => "px"
  | Percent => "%";
