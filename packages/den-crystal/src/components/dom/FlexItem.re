open Style;

let component = ReasonReact.statelessComponent("FlexItem");

type style = {
  paddingRight: string,
  paddingBottom: string,
};

let getStyle = (~gap, ~grow) => {
  let padding = gap |> rem_of_int;
  ReactDOMRe.Style.make(
    ~paddingRight=padding,
    ~paddingBottom=padding,
    ~flex=grow ? "1" : "",
    (),
  );
};

let make = (~gap=0, ~grow=false, ~className: option(string)=?, children) => {
  ...component,
  render: _self =>
    <div ?className style={getStyle(~gap, ~grow)}> ...children </div>,
};
