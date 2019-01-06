let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("PagePacksByTag");

let make = (~tag, ~packs: array(Data.Pack.t), _children) => {
  ...component,
  render: _self => <PageCarbonCards packs title={"Packs by tag: " ++ tag} />,
};
