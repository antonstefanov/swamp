let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("PageCarbon");

let handleKeyDown = (event: ReactEvent.Keyboard.t) => {
  switch (
    ReactEvent.Keyboard.key(event),
    ReactEvent.Keyboard.metaKey(event),
    ReactEvent.Keyboard.altKey(event),
    ReactEvent.Keyboard.shiftKey(event),
  ) {
  | ("c", false, false, false) => true
  | _ => false
  };
};

let make = (~pack as providedPack: option(Data.Pack.t), ~id, _children) => {
  ...component,
  render: _self =>
    <Layout>
      <Flex direction=Column container=Centered>
        <Page.Title> {str("carbon/")} <Text.Select value=id /> </Page.Title>
        {switch (providedPack) {
         | None =>
           <div>
             {str("Pack not found. Ensure you have the latest swamp.")}
           </div>
         | Some(pack) =>
           <Flex direction=Column gap=1>
             <CodeCopy
               value={"o add " ++ pack.title}
               copyOnKeyDown=handleKeyDown
               title="(c)opy"
             />
             <Page.Description> {str(pack.description)} </Page.Description>
             <CarbonTags tags={pack.tags} />
             <Markdown path={pack.readme} />
           </Flex>
         }}
      </Flex>
    </Layout>,
};
