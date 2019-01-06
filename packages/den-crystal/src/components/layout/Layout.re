let component = ReasonReact.statelessComponent("Layout");

/* let make = (~header, ~aside, ~footer, children) => { */
let make = children => {
  ...component,
  render: _self =>
    <World> <Header /> <Main> ...children </Main> <Footer /> </World>,
};
