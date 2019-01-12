open Style;
Assets.requireCSS("./Flex.scss");

let b = Bem.make("Flex");
let bItem = Bem.make(b.element("Item"));

let component = ReasonReact.statelessComponent("Flex");

let negative = text => "-" ++ text;

let some = value => Some(value);

type alignX =
  | Default
  | Left
  | Right;

type alignY =
  | Default
  | Top
  | Bottom;

let gap_of_int =
  fun
  | None => None
  | Some(value) => value |> rem_of_int |> negative |> some;

type itemsPerRow =
  | Any
  | Items1
  | Items2
  | Items3
  | Items4
  | Items5;

let string_of_itemsPerRow =
  fun
  | Any => "i-any"
  | Items1 => "i-1"
  | Items2 => "i-2"
  | Items3 => "i-3"
  | Items4 => "i-4"
  | Items5 => "i-5";

type direction =
  | Row(itemsPerRow)
  | Column;

let itemsPerRow_of_direction =
  fun
  | Row(itemsPerRow) => Some(itemsPerRow)
  | Column => None;

let sizedItemsPerRow = (size, direction) =>
  direction
  |> itemsPerRow_of_direction
  |> Maybe.map(ipr => size ++ "-" ++ string_of_itemsPerRow(ipr));

let string_of_direction =
  fun
  | Row(_) => "row"
  | Column => "column";

type wrap =
  | Wrap
  | NoWrap;

let string_of_wrap =
  fun
  | Wrap => "wrap"
  | NoWrap => "nowrap";

type align =
  | Default /* Normal alignment */
  | Center /* Pack items around the center */
  | FlexStart /*  Pack flex items from the start */
  | FlexEnd /*  Pack flex items from the end */
  | SpaceBetween /* Distribute items evenly
                    The first item is flush with the start,
                    the last is flush with the end */
  | SpaceAround /* Distribute items evenly
                   Items have a half-size space
                   on either end */
  | SpaceEvenly /* Distribute items evenly
                   Items have equal space around them */
  | Stretch; /* Distribute items evenly
               Stretch 'auto'-sized items to fit
               the container */

let string_of_align =
  fun
  | Default => "normal"
  | Center => "center"
  | FlexStart => "flex-start"
  | FlexEnd => "flex-end"
  | SpaceBetween => "space-between"
  | SpaceAround => "space-around"
  | SpaceEvenly => "space-evenly"
  | Stretch => "stretch";

type alignItems =
  | Default
  | Center
  | FlexStart
  | FlexEnd;

let string_of_align_items =
  fun
  | Default => "normal"
  | Center => "center"
  | FlexStart => "flex-start"
  | FlexEnd => "flex-end";

type container =
  | Normal
  | Centered
  | Full;

let getStyle =
    (~gap=?, ~justify, ~alignItems, ~alignContent, ~wrap, ~container) => {
  let gap = gap |> gap_of_int;
  ReactDOMRe.Style.make(
    ~display="flex",
    ~marginRight=?
      switch (container) {
      | Normal => gap
      | Centered => None
      | Full => None
      },
    ~marginBottom=?gap,
    ~flexWrap=string_of_wrap(wrap),
    ~justifyContent=string_of_align(justify),
    ~alignContent=string_of_align(alignContent),
    ~alignItems=string_of_align_items(alignItems),
    (),
  );
};

let string_of_container =
  fun
  | Normal => "normal"
  | Centered => "container"
  | Full => "container-full";

let sizedDirection = (size, direction) =>
  size ++ "-" ++ (direction |> string_of_direction);

type items =
  | GrowLast;

let make =
    (
      ~gap: option(int)=?,
      ~gapContainer: option(int)=?,
      ~direction=Row(Any),
      ~usm=?,
      ~sm=?,
      ~md=?,
      ~lg=?,
      ~xl=?,
      ~justify: align=Default,
      ~alignItems: alignItems=Default,
      ~alignContent: align=Default,
      ~wrap=NoWrap,
      ~container=Normal,
      ~className=?,
      ~items: option(items)=?,
      children,
    ) => {
  let growLast =
    items |> Maybe.map(x => x === GrowLast) |> Maybe.withDefault(false);
  let lastIndex = Array.length(children) - 1;
  let isLast = i => i === lastIndex;
  let renderItems = () =>
    children
    |> Array.mapi((i, child) =>
         <FlexItem
           className={Cn.make([
             bItem.className,
             direction
             |> itemsPerRow_of_direction
             |> Maybe.map(string_of_itemsPerRow)
             |> bItem.modifierIfSome,
             usm
             |> Maybe.mapFlat(sizedItemsPerRow("usm"))
             |> bItem.modifierIfSome,
             sm
             |> Maybe.mapFlat(sizedItemsPerRow("sm"))
             |> bItem.modifierIfSome,
             md
             |> Maybe.mapFlat(sizedItemsPerRow("md"))
             |> bItem.modifierIfSome,
             lg
             |> Maybe.mapFlat(sizedItemsPerRow("lg"))
             |> bItem.modifierIfSome,
             xl
             |> Maybe.mapFlat(sizedItemsPerRow("xl"))
             |> bItem.modifierIfSome,
           ])}
           key={string_of_int(i)}
           ?gap
           grow={growLast && isLast(i)}>
           child
         </FlexItem>
       )
    |> ReasonReact.array;
  {
    ...component,
    render: _self =>
      <div
        className=Cn.(
          make([
            b.className,
            container |> string_of_container |> b.modifier,
            className |> unwrap,
            direction |> string_of_direction |> b.modifier,
            usm |> Maybe.map(sizedDirection("usm")) |> b.modifierIfSome,
            sm |> Maybe.map(sizedDirection("sm")) |> b.modifierIfSome,
            md |> Maybe.map(sizedDirection("md")) |> b.modifierIfSome,
            lg |> Maybe.map(sizedDirection("lg")) |> b.modifierIfSome,
            xl |> Maybe.map(sizedDirection("xl")) |> b.modifierIfSome,
          ])
        )
        style={getStyle(
          ~gap=?Belt.Option.mapWithDefault(gapContainer, gap, g => Some(g)),
          ~justify,
          ~alignItems,
          ~alignContent,
          ~wrap,
          ~container,
        )}>
        {renderItems()}
      </div>,
  };
};
