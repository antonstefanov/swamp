Assets.requireCSS("./IconBase.scss");

let component = ReasonReact.statelessComponent("IconBase");
let make =
    (
      ~name: string,
      ~className=?,
      ~color=?,
      ~rotate=?,
      ~size=?,
      ~id=?,
      ~viewBox="0 0 1 1",
      ~animation=?,
      ~align=false,
      ~spaced=false,
      ~maximize=false,
      ~soloMax=false,
      ~contour=false,
      ~stroke=false,
      ~hover=false,
      ~animate=false,
      ~transparent=false,
      ~flipX=false,
      ~flipY=false,
      children,
    ) => {
  ...component,
  render: _self =>
    <i
      className={IconGeneration.getGeneratedIconClass(
        ~animation?,
        ~align,
        ~spaced,
        ~maximize,
        ~soloMax,
        ~name,
        ~className?,
        ~hover,
        ~animate,
        ~contour,
        ~stroke,
        ~transparent,
        ~size?,
        (),
      )}
      ?id
      ariaHidden=true
      role="img">
      <svg
        className=IconGeneration.icon_svg_class
        style={IconGeneration.getStyle(~color?, ~rotate?, ~flipX, ~flipY, ())}
        viewBox>
        ...children
      </svg>
    </i>,
};
