let str = ReasonReact.string;

let tip =
  <Page.Description>
    {str("Use ")}
    <Kbd> {str("t")} </Kbd>
    {str(" to go to the search page, ")}
    <Kbd> Kbd.cmd <Kbd> {str("+")} </Kbd> <Kbd> {str("[")} </Kbd> </Kbd>
    {str(" to go back and ")}
    <Kbd> Kbd.cmd <Kbd> {str("+")} </Kbd> <Kbd> {str("[")} </Kbd> </Kbd>
    {str(" to go forward.")}
  </Page.Description>;

let component = ReasonReact.statelessComponent("PageCarbonSearch");
let make =
    (
      ~search: string,
      ~jsonPacks: array(Data.Json.t),
      ~pushSearch: string => unit,
      _children,
    ) => {
  let updatePath = value => value != search ? pushSearch(value) : ();
  {
    ...component,
    render: _self =>
      <Layout>
        <Flex direction=Column container=Centered>
          <Page.Title> {str("tearch")} </Page.Title>
          tip
          <CarbonSearch
            jsonPacks
            search
            onBlur=updatePath
            onSubmit=updatePath
          />
        </Flex>
      </Layout>,
  };
};
