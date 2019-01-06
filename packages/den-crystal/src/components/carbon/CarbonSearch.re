Assets.requireCSS("./CarbonSearch.scss");

let b = Bem.make("CarbonSearch");

let str = ReasonReact.string;

let renderItems = (packs: array(Data.Json.t), _search) =>
  <CarbonCards packs={Data.ofPacks(packs)} />;

let bInput = b.element("input-container") |> Bem.make;
let getInputContainerClassName = (~showItems) =>
  Cn.(
    make([
      bInput.className,
      showItems |> bInput.modifierIfTrue("show-items"),
    ])
  );

module InternalCarbonSearch =
  Search.Make({
    type t = Data.Json.t;
    let name = "CarbonSearch";
    let inputClassName = b.element("input");
    let render = (~input, ~items as packs, ~searchValue) =>
      <Flex direction=Column gap=1 className={b.className}>
        <div className={b.element("input-container")}> input </div>
        <div className={b.element("items")}>
          {renderItems(packs, searchValue)}
        </div>
      </Flex>;

    let fuseOptions =
      Fuse.Options.make(
        ~keys=
          Fuse.Options.(
            [|
              W("title", 1.0),
              W("description", 0.55),
              W("tags", 0.40),
              W("images", 0.15),
            |]
          ),
        (),
      );
  });

let component = ReasonReact.statelessComponent("CarbonSearch");

let make =
    (
      ~jsonPacks: array(Data.Json.t),
      ~onBlur=?,
      ~onSubmit=?,
      ~search,
      _children,
    ) => {
  ...component,
  render: _self =>
    <Flex direction=Column container=Normal gap=1 className={b.className}>
      <InternalCarbonSearch
        autoFocus=true
        placeholder="Search for example 'files'"
        items=jsonPacks
        search
        ?onBlur
        ?onSubmit
      />
    </Flex>,
};
