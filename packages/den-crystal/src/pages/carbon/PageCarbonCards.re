let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("PageCarbonCards");

let make = (~title: string, ~packs: array(Data.Pack.t), _children) => {
  ...component,
  render: _self =>
    <Layout>
      <Flex direction=Column container=Centered>
        <Page.Title> {str(title)} </Page.Title>
        <CarbonCards packs gapContainer=0 />
      </Flex>
    </Layout>,
};
