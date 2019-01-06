let rec join = (delimiter, list) =>
  switch (list) {
  | [] => ""
  | [selector] => selector
  | [hd, ...tl] => hd ++ delimiter ++ join(delimiter, tl)
  };

let rec contains = (content, list) =>
  switch (list) {
  | [] => false
  | [hd, ...tl] => hd == content || contains(content, tl)
  };

let fold_right = (fn, default, list) => List.fold_right(fn, list, default);
