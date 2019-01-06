let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("PageHome");

let make = (~packs: array(Data.Pack.t), _children) => {
  ...component,
  render: _self => <PageCarbonCards packs title="Home" />,
};
