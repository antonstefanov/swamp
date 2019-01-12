Assets.requireCSS("./CarbonCards.scss");

let b = Bem.make("CarbonCards");

let component = ReasonReact.statelessComponent("CarbonCards");

let str = ReasonReact.string;

let getCardImg = images =>
  Array.length(images) > 0 ? images[0] : "/images/defaultCarbon.png";

let renderCard = (pack: Data.Pack.t) =>
  <Card
    key={pack.id}
    href={"/carbon/pack/" ++ pack.id}
    title={pack.title}
    description={pack.description}
    imageSrc={getCardImg(pack.images)}
    className={b.element("item")}>
    <CarbonTags tags={pack.tags} />
  </Card>;

let make =
    (
      ~packs: array(Data.Pack.t),
      ~gap=1,
      ~gapContainer=?,
      ~className=?,
      _children,
    ) => {
  ...component,
  render: _self =>
    <Flex
      direction={Row(Items1)}
      usm={Row(Items2)}
      sm={Row(Items2)}
      md={Row(Items3)}
      lg={Row(Items4)}
      container=Normal
      gap
      ?gapContainer
      ?className
      wrap=Wrap>
      ...{packs |> Array.map(renderCard)}
    </Flex>,
};
