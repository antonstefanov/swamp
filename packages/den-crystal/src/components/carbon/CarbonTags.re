let str = ReasonReact.string;
let getLink = tag => "/carbon/tag/" ++ tag;

let component = ReasonReact.statelessComponent("CarbonTags");

let make = (~tags, _children) => {
  ...component,
  render: _self => <Tags tags getLink />,
};
