Assets.requireCSS("./Loading.scss");

let b = Bem.make("Loading");
let str = ReasonReact.string;

let dot = index =>
  <span className={b.element("dot-" ++ string_of_int(index))}>
    {str(".")}
  </span>;

let component = ReasonReact.statelessComponent("Loading");

let make = (~text="Loading", _children) => {
  ...component,
  render: _self =>
    <Flex className={b.className} gap=1 justify=Center>
      <span className={b.element("text")}> {str(text)} </span>
      <span className={b.element("dots")}>
        {dot(1)}
        {dot(2)}
        {dot(3)}
      </span>
    </Flex>,
};
