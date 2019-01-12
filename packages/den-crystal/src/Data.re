include DenSeed.Packs;

[@bs.val] external require: string => JsInterop.Output.t = "require";

let parse = json => JsInterop.Output.ofJson(json);

let sortDataPacks = data => {
  data.packs
  |> Array.sort((a: Pack.t, b: Pack.t) =>
       b.modified -. a.modified |> int_of_float
     );
  data;
};

let fetch = path =>
  path
  |> BetterFetch.fetchJson
  |> BetterFetch.thenMap(result =>
       result |> parse |> make |> sortDataPacks |> Js.Promise.resolve
     );

let dirtyFetch = Persist.CacheAsync.make(fetch);

let reduceToIdMap = (packs: list(Pack.t)) =>
  packs
  |> List.fold_left(
       (acc, pack: Pack.t) => acc->Belt.Map.String.set(pack.id, pack),
       Belt.Map.String.empty,
     );

let reduceToTagMap = (packs: list(Pack.t)) =>
  packs
  |> List.fold_left(
       (acc, pack: Pack.t) =>
         pack.tags
         |> Array.fold_left(
              (nextAcc, tag) => {
                let packs = nextAcc->Belt.Map.String.getWithDefault(tag, []);
                nextAcc->Belt.Map.String.set(tag, [pack, ...packs]);
              },
              acc,
            ),
       Belt.Map.String.empty,
     )
  |> Belt.Map.String.toList
  |> List.fold_left(
       (acc, (tag, packs)) =>
         Belt.Map.String.set(acc, tag, packs |> List.rev |> Array.of_list),
       Belt.Map.String.empty,
     );

let packById = (packs: list(Pack.t), id: string) => {
  let rec aux = (packs: list(Pack.t)) =>
    switch (packs) {
    | [] => None
    | [x, ...xs] => x.id === id ? Some(x) : aux(xs)
    };
  aux(packs);
};

let hasTag = (tags: array(string), tag: string) => {
  let len = Array.length(tags);
  let rec aux = i => i < len && (tags[i] === tag || aux(i + 1));
  aux(0);
};

/* O(m * n) ftw */
let packsByTag = (packs: list(Pack.t), tag: string) =>
  packs
  |> List.fold_left(
       (acc, p: Pack.t) => hasTag(p.tags, tag) ? [p, ...acc] : acc,
       [],
     )
  |> List.rev
  |> Array.of_list;
