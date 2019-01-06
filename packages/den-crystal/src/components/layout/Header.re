Assets.requireCSS("./Header.scss");

let b = Bem.make("Header");
let nav = Bem.make("navbar");

let component = ReasonReact.statelessComponent("Header");

let str = ReasonReact.string;
let make = _children => {
  ...component,
  render: _self =>
    <Navbar className={b.className} theme=Light>
      <Link
        href="/"
        title="Home"
        className={Cn.make([Navbar.item, b.element("logo-link")])}>
        <Logo className={b.element("logo")} />
        {ReasonReact.string("home")}
      </Link>
    </Navbar>,
};
