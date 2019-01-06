let add = (a, b) => a + b;

let endsWithChar = (text, char) =>
  text |> String.length |> add(-1) |> String.get(text) == char;

let endsWith = (text, endText) =>
  switch (String.length(text), String.length(endText)) {
  | (tlen, elen) when tlen < elen => false
  | (tlen, elen) when tlen == elen => text == endText
  | (tlen, elen) => text->String.sub(tlen - elen, elen) === endText
  };

let startsWithChar = (text, char) => text.[0] === char;

let startsWith = (text, startText) =>
  switch (String.length(text), String.length(startText)) {
  | (tlen, elen) when tlen < elen => false
  | (tlen, elen) when tlen == elen => text == startText
  | (_tlen, elen) => text->String.sub(0, elen) === startText
  };
