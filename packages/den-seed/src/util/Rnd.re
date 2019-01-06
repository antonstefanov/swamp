Random.self_init();

let range = (min, max) => {
  let rec aux = (acc, min', max') =>
    if (min' > max') {
      acc;
    } else {
      aux([min', ...acc], min' + 1, max');
    };

  if (min < max) {
    List.rev(aux([], min, max));
  } else {
    aux([], min, max);
  };
};

let rand_pick = arr => arr |> Array.length |> Random.int |> Array.get(arr);

let rand_select = (list, n) => {
  let rec extract = (acc, n) =>
    fun
    | [] => raise(Not_found)
    | [h, ...t] =>
      if (n == 0) {
        (h, acc @ t);
      } else {
        extract([h, ...acc], n - 1, t);
      };

  let extract_rand = (list, len) => extract([], Random.int(len), list);

  let rec aux = (n, acc, list, len) =>
    if (n == 0) {
      acc;
    } else {
      let (picked, rest) = extract_rand(list, len);
      aux(n - 1, [picked, ...acc], rest, len - 1);
    };

  let len = List.length(list);
  aux(min(n, len), [], list, len);
};

let lotto_select = (n, m) => rand_select(range(1, m), n);
