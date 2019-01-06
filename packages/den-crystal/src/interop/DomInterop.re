[@bs.val] external document: Dom.document = "";
[@bs.val] external window: Dom.window = "";

[@bs.send]
external addEventListener:
  (Dom.document, string, Dom.event_like('a) => bool, bool) => unit =
  "";

[@bs.send] external focus: Dom.element => unit = "";
[@bs.send] external blur: Dom.element => unit = "";
[@bs.send] external select: Dom.element => unit = "";

[@bs.send] external scrollTo: (Dom.window, int, int) => unit = "";

[@bs.send] external execCommand: (Dom.document, string) => unit = "";
