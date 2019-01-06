module Options = {
  [@bs.deriving abstract]
  type _key = {
    name: string,
    weight: float,
  };
  [@bs.deriving abstract]
  type t = {
    /* Approximately where in the text is the pattern expected to be found? */
    [@bs.optional]
    location: int,
    /* Determines how close the match must be to the fuzzy location (specified above).
       An exact letter match which is 'distance' characters away from the fuzzy location
       would score as a complete mismatch. A distance of '0' requires the match be at
       the exact location specified, a threshold of '1000' would require a perfect match
       to be within 800 characters of the fuzzy location to be found using a 0.8 threshold. */
    [@bs.optional]
    distance: int,
    /* At what point does the match algorithm give up. A threshold of '0.0' requires a perfect match
       (of both letters and location), a threshold of '1.0' would match anything. */
    [@bs.optional]
    threshold: float,
    /* Machine word size */
    [@bs.optional]
    maxPatternLength: int,
    /* Indicates whether comparisons should be case sensitive. */
    [@bs.optional]
    caseSensitive: bool,
    /* Regex used to separate words when searching. Only applicable when `tokenize` is `true`. */
    [@bs.optional]
    tokenSeparator: Js.Re.t,
    /* When true, the algorithm continues searching to the end of the input even if a perfect
       match is found before the end of the same input. */
    [@bs.optional]
    findAllMatches: bool,
    /* Minimum number of characters that must be matched before a result is considered a match */
    [@bs.optional]
    minMatchCharLength: int,
    /* The name of the identifier property. If specified, the returned result will be a list
       of the items' dentifiers, otherwise it will be a list of the items. */
    [@bs.optional]
    id: Js.Null.t(string),
    /* List of properties that will be searched. This also supports nested properties. */
    [@bs.optional]
    keys: array(_key),
    /* Whether to sort the result list, by score */
    [@bs.optional]
    shouldSort: bool,
    /* When true, the search algorithm will search individual words **and** the full string,
       computing the final score as a function of both. Note that when `tokenize` is `true`,
       the `threshold`, `distance`, and `location` are inconsequential for individual tokens. */
    [@bs.optional]
    tokenize: bool,
    [@bs.optional]
    /* When true, the result set will only include records that match all tokens. Will only work
       if `tokenize` is also true. */
    matchAllTokens: bool,
    /* Will print to the console. Useful for debugging. */
    [@bs.optional]
    verbose: bool,
    [@bs.optional]
    includeMatches: bool,
    [@bs.optional]
    includeScore: bool,
  };

  type key =
    | K(string)
    | W(string, float);

  type weight =
    | Standard
    | Tokenize
    | MatchAll;

  let tokenize_of_weight =
    fun
    | Standard => false
    | Tokenize => true
    | MatchAll => true;

  let match_all_of_weight =
    fun
    | Standard => false
    | Tokenize => false
    | MatchAll => true;

  let stringkey_of_key =
    fun
    | K(key) => _key(~name=key, ~weight=1.0)
    | W(key, weight) => _key(~name=key, ~weight);

  let make =
      (
        ~location=0,
        ~distance=100,
        ~threshold=0.6,
        ~maxPatternLength=32,
        ~caseSensitive=false,
        ~tokenSeparator=Js.Re.fromStringWithFlags(" +", ~flags="g"),
        ~findAllMatches=false,
        ~minMatchCharLength=1,
        ~id: option(string)=None,
        ~keys: array(key),
        ~shouldSort=true,
        ~weight=Standard,
        ~includeMatches=false,
        ~includeScore=false,
        ~verbose=false,
        (),
      ) =>
    t(
      ~location,
      ~distance,
      ~threshold,
      ~maxPatternLength,
      ~caseSensitive,
      ~tokenSeparator,
      ~findAllMatches,
      ~minMatchCharLength,
      ~id=Js.Null.fromOption(id),
      ~keys=Array.map(stringkey_of_key, keys),
      ~shouldSort,
      ~includeMatches,
      ~includeScore,
      ~verbose,
      ~tokenize=tokenize_of_weight(weight),
      ~matchAllTokens=match_all_of_weight(weight),
      (),
    );
};

type t('a);
[@bs.module] [@bs.new]
external make: (array('a), Options.t) => t('a) = "fuse.js";

[@bs.send] external search: (t('a), string) => array('a) = "";
