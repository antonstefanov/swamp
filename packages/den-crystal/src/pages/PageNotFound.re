let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("PageNotFound");

let make = _children => {
  ...component,
  render: _self =>
    <Layout>
      <Flex direction=Column container=Centered>
        <Page.Title> {str("Not found")} </Page.Title>
        <div>
          <span>
            {str("The page you're looking for can't be found. Go home by ")}
          </span>
          <Link href="/" title="Home"> {str("clicking here!")} </Link>
        </div>
      </Flex>
    </Layout>,
};
