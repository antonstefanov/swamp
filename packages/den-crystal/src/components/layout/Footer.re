Assets.requireCSS("./Footer.scss");

let str = ReasonReact.string;

let currentYear = Js.Date.make() |> Js.Date.getFullYear |> string_of_float;

let b = Bem.make("Footer");

let component = ReasonReact.statelessComponent("Footer");

let make = _children => {
  ...component,
  render: _self =>
    <footer className={b.className}>
      <Flex container=Centered alignItems=Center>
        <Logo.Carbon size=Md align=true />
        <div> {str("carbon & crystal")} </div>
        <Logo.Crystal size=Md align=true />
      </Flex>
    </footer>,
};
