module type LogoGen = {
  let logo: ReasonReact.reactElement;
  let text: ReasonReact.reactElement;
  let name: string;
};
module LogoGen = (G: LogoGen) => {
  let render = showText =>
    <g> G.logo {showText ? G.text : ReasonReact.null} </g>;

  let component = ReasonReact.statelessComponent(G.name);
  let make =
      (
        ~text=true,
        ~className=?,
        ~rotate=?,
        ~size=?,
        ~id=?,
        ~animation=?,
        ~align=?,
        ~maximize=?,
        ~soloMax=?,
        ~contour=?,
        ~stroke=?,
        ~hover=?,
        ~animate=?,
        ~transparent=?,
        ~flipX=?,
        ~flipY=?,
        _children,
      ) => {
    {
      ...component,
      render: _self =>
        <IconBase
          name="logo-carbon"
          ?className
          ?animation
          ?align
          ?rotate
          ?size
          ?id
          viewBox="0 0 256 189"
          ?maximize
          ?soloMax
          ?contour
          ?stroke
          ?hover
          ?animate
          ?transparent
          ?flipX
          ?flipY>
          {render(text)}
        </IconBase>,
    };
  };
};
module Carbon =
  LogoGen({
    let name = "LogoCarbon";
    let logo =
      <g>
        <path
          d="M0 94.5l94.326 94.326 23.582-23.581-94.326-94.327L0 94.5z"
          fill="#3c3c3c"
        />
        <path
          d="M23.582 118.082l94.326-94.327L94.326.174 0 94.5l23.582 23.582z"
          fill="gray"
        />
      </g>;

    let text =
      <path
        fill="gray"
        d="M63.629 147.405c-.271.272-.404.558-.399.857.006.3.139.58.399.841.328.328.642.45.941.365.3-.085.558-.212.773-.382l1.73 1.732c-.758.758-1.601 1.136-2.529 1.136a3.452 3.452 0 0 1-1.375-.255 3.807 3.807 0 0 1-1.272-.866c-.543-.543-.89-1.118-1.043-1.723a3.311 3.311 0 0 1 .026-1.773c.102-.351.278-.696.527-1.035.249-.34.549-.685.899-1.035l2.836-2.833a8.96 8.96 0 0 1 1.035-.899c.34-.249.685-.424 1.036-.526a3.301 3.301 0 0 1 1.773-.025c.605.153 1.179.501 1.722 1.045.396.396.687.823.874 1.281.186.458.274.92.262 1.383-.057.962-.493 1.85-1.308 2.664l-1.73-1.731c.351-.351.532-.685.544-1.001.011-.317-.113-.606-.373-.866-.261-.26-.549-.385-.866-.374-.317.011-.639.181-.967.509l-3.515 3.511zM68.869 157.607l.883-.882-.034-.034c-.543.09-1.015.104-1.417.042-.401-.063-.812-.303-1.23-.722a3.05 3.05 0 0 1-.526-.713 2.277 2.277 0 0 1-.254-.951c-.034-.35.04-.735.221-1.153.181-.419.51-.866.985-1.341a8.742 8.742 0 0 1 1.4-1.162c.447-.288.903-.449 1.367-.483.441-.034.896.066 1.366.298.469.232.975.62 1.518 1.163l.187.187c.068.067.124.135.169.203.046.068.108.142.187.221.079.079.158.17.237.272l.9-.9c.295-.294.476-.599.544-.916.068-.316-.062-.639-.39-.967a1.374 1.374 0 0 0-.84-.399c-.334-.04-.67.053-1.01.279l-1.679-1.68c.814-.724 1.697-1.086 2.647-1.086.453.001.903.094 1.349.281a3.94 3.94 0 0 1 1.264.874c.362.363.645.759.848 1.189.204.43.311.877.322 1.341 0 .452-.102.916-.306 1.391-.203.475-.532.939-.984 1.391l-5.993 5.988-1.731-1.731zm3.701-3.698a3.947 3.947 0 0 0-.424-.527c-.373-.373-.78-.605-1.221-.696-.442-.091-.894.096-1.358.56-.328.328-.515.673-.561 1.035-.045.362.074.684.356.967.294.294.62.427.976.399.356-.028.71-.217 1.061-.568l1.171-1.17zM71.872 160.612l8.607-8.601 1.731 1.732-.917.916a7.41 7.41 0 0 1 1.782.289c.52.159 1.029.487 1.527.985l-1.834 1.832a1.984 1.984 0 0 0-.424-.628 2.936 2.936 0 0 0-.679-.493 2.139 2.139 0 0 0-.882-.254 2.315 2.315 0 0 0-1.018.135c-.351.124-.702.362-1.053.712l-5.11 5.106-1.73-1.731zM76.588 165.331l12.088-12.078 1.73 1.732-4.261 4.258.034.034a2.952 2.952 0 0 1 1.34.034c.419.113.798.34 1.137.679.418.419.67.818.755 1.197.084.379.087.755.008 1.129-.068.294-.232.605-.493.933-.26.328-.701.803-1.324 1.425l-3.277 3.274c-.475.475-.877.831-1.205 1.069a3.249 3.249 0 0 1-.933.491c-.838.249-1.613.017-2.325-.696-.43-.43-.673-.843-.729-1.239a5.993 5.993 0 0 1-.034-1.29l-.781.78-1.73-1.732zm9.403-2.508c.271-.271.45-.568.534-.89.085-.323-.036-.648-.364-.976a1.15 1.15 0 0 0-.882-.357c-.34.011-.645.153-.917.424l-3.344 3.342c-.317.316-.496.653-.535 1.009-.04.357.07.665.33.925.294.295.617.402.967.323.351-.079.702-.294 1.053-.644l3.158-3.156zM88.756 166.066c.35-.351.696-.65 1.035-.899.34-.249.685-.424 1.035-.526a3.315 3.315 0 0 1 1.774-.025c.605.153 1.179.501 1.722 1.045.543.543.891 1.117 1.043 1.723a3.301 3.301 0 0 1-.026 1.773 3.317 3.317 0 0 1-.527 1.035 9.159 9.159 0 0 1-.899 1.035l-2.836 2.833c-.35.351-.696.65-1.035.899-.34.249-.685.424-1.036.526a3.311 3.311 0 0 1-1.773.024c-.605-.153-1.179-.501-1.722-1.044-.543-.543-.891-1.118-1.043-1.723a3.301 3.301 0 0 1 .026-1.773c.102-.351.277-.696.526-1.035.25-.34.549-.685.9-1.035l2.836-2.833zm-1.377 4.836c-.328.328-.498.65-.509.967-.012.317.112.605.373.866.26.26.548.385.865.373.317-.011.639-.18.967-.508l3.379-3.376c.328-.328.498-.65.509-.967.012-.317-.113-.606-.373-.866s-.548-.385-.865-.374c-.317.012-.639.181-.968.509l-3.378 3.376zM89.464 178.217l8.607-8.601 1.73 1.732-.832.831.034.034c.453-.09.894-.096 1.324-.016.43.079.843.317 1.239.713.215.215.393.467.534.755.141.289.212.603.212.942 0 .34-.083.693-.247 1.061-.164.368-.439.744-.823 1.128l-6.604 6.599-1.731-1.732 5.959-5.954c.317-.317.493-.639.527-.967.034-.328-.085-.628-.356-.9-.328-.328-.656-.452-.984-.373-.329.079-.685.311-1.07.695l-5.789 5.785-1.73-1.732z"
      />;
  });

module Crystal =
  LogoGen({
    let name = "LogoCrystal";
    let logo =
      <g>
        <path
          d="M256 94.5L161.674.174l-23.582 23.581 94.326 94.327L256 94.5z"
          fill="#6c1ca5"
        />
        <path
          d="M232.418 70.918l-94.326 94.327 23.582 23.581L256 94.5l-23.582-23.582z"
          fill="#8b40bf"
        />
      </g>;

    let text =
      <path
        fill="#9d6cc0"
        d="M191.469 44.873c.272-.272.405-.557.4-.857-.006-.3-.139-.58-.399-.84-.327-.329-.641-.451-.941-.366-.3.085-.558.212-.773.382l-1.73-1.732c.759-.758 1.602-1.137 2.53-1.136.463-.011.922.074 1.374.255.453.182.877.47 1.272.866.543.544.891 1.118 1.043 1.724a3.301 3.301 0 0 1-.026 1.773 3.317 3.317 0 0 1-.527 1.035 9.073 9.073 0 0 1-.9 1.035l-2.836 2.832c-.351.351-.696.65-1.035.899-.34.249-.685.424-1.036.525a3.301 3.301 0 0 1-1.773.025c-.605-.153-1.179-.501-1.722-1.045a3.878 3.878 0 0 1-.873-1.281 3.42 3.42 0 0 1-.263-1.384c.057-.961.493-1.849 1.308-2.663l1.73 1.732c-.35.35-.532.684-.543 1.001-.012.316.113.605.373.865.26.261.548.385.865.374.317-.011.639-.181.968-.508l3.514-3.511zM189.505 37.947l-8.609 8.6-1.73-1.732.917-.916a7.391 7.391 0 0 1-1.782-.29c-.52-.158-1.029-.487-1.527-.985l1.834-1.831c.102.237.243.447.424.628s.407.345.679.492c.271.147.565.232.882.255.328.034.667-.011 1.018-.135.351-.124.702-.362 1.053-.712l5.111-5.105 1.73 1.731zM176.605 42.251l-1.832-1.834 4.178-6.548-.034-.034-6.536 4.188-1.781-1.783 12.58-7.613c.475-.294.919-.483 1.332-.568a3.353 3.353 0 0 1 1.18-.041c.373.056.712.175 1.018.357.305.181.588.402.848.662l.475.475-1.63 1.629-.39-.391a1.84 1.84 0 0 0-.509-.374c-.17-.079-.385-.09-.645-.034-.249.068-.557.206-.925.415s-.857.518-1.469.924l-5.86 10.57zM169.178 29.994c-.283.305-.41.619-.382.942.028.322.155.597.381.823.272.272.569.393.891.365.323-.028.597-.155.823-.381.159-.158.275-.353.349-.585.073-.232.008-.574-.195-1.027l-.61-1.426c-.396-.894-.5-1.678-.313-2.351a3.973 3.973 0 0 1 1.045-1.772 3.65 3.65 0 0 1 1.205-.814 3.791 3.791 0 0 1 1.358-.288c.464-.011.922.074 1.375.255.452.181.865.459 1.238.832.701.702 1.068 1.511 1.102 2.428.022.452-.055.913-.23 1.383-.176.469-.473.924-.892 1.365l-1.628-1.63c.204-.271.342-.574.416-.907.074-.334-.053-.665-.381-.993a1.446 1.446 0 0 0-.916-.425c-.362-.034-.691.096-.985.39a1.421 1.421 0 0 0-.425.797c-.045.294.034.668.237 1.12l.526 1.138c.339.746.463 1.476.372 2.189-.091.713-.425 1.357-1.002 1.934a3.55 3.55 0 0 1-1.214.806 3.488 3.488 0 0 1-1.366.245c-.453 0-.891-.094-1.315-.281a3.807 3.807 0 0 1-1.163-.807 3.563 3.563 0 0 1-.788-1.163 3.146 3.146 0 0 1-.245-1.298c.011-.441.11-.88.297-1.315.187-.435.456-.828.807-1.179l1.628 1.63zM163.935 29.567l-2.615 2.612-1.73-1.732 2.615-2.612-1.069-1.07 1.358-1.357 1.069 1.07 4.771-4.766c.204-.204.343-.382.416-.534a.666.666 0 0 0 .06-.45 1.044 1.044 0 0 0-.229-.433 8.24 8.24 0 0 0-.466-.501l1.63-1.628.712.713c.396.396.665.79.806 1.18.141.39.194.761.16 1.112a2.64 2.64 0 0 1-.306 1.001 3.46 3.46 0 0 1-.594.814l-5.23 5.224.865.866-1.358 1.357-.865-.866zM165.81 14.227l-.883.882.034.033c.543-.09 1.015-.104 1.417-.041.402.062.812.303 1.23.722.203.204.379.441.526.713.147.271.231.588.254.95.033.351-.04.736-.222 1.154-.181.419-.509.865-.985 1.34a8.594 8.594 0 0 1-1.4 1.162c-.447.288-.903.449-1.367.483-.441.034-.896-.066-1.366-.298-.469-.232-.975-.62-1.518-1.163l-.187-.187a1.382 1.382 0 0 1-.169-.204 1.474 1.474 0 0 0-.187-.22 3.001 3.001 0 0 1-.237-.272l-.9.899c-.294.294-.476.599-.544.916-.068.317.062.639.39.968.226.226.506.359.84.399.334.04.67-.054 1.01-.28l1.679 1.681c-.815.724-1.698 1.086-2.648 1.085-.452 0-.902-.094-1.349-.281a3.95 3.95 0 0 1-1.264-.874 4.273 4.273 0 0 1-.848-1.189 3.293 3.293 0 0 1-.321-1.341c0-.452.102-.916.306-1.391.204-.475.532-.939.985-1.391l5.994-5.987 1.73 1.732zm-3.702 3.697c.159.226.3.402.424.526.373.374.781.606 1.222.697.441.091.894-.096 1.358-.559.328-.328.515-.673.56-1.035.046-.362-.073-.685-.356-.968-.294-.294-.619-.427-.975-.399-.357.028-.71.217-1.061.568l-1.172 1.17zM150.82 23.399l-1.73-1.732 9.661-9.651c.329-.327.493-.61.493-.848 0-.237-.147-.537-.441-.899l1.63-1.629c.396.396.721.784.975 1.163.255.379.405.773.45 1.18.056.396-.012.809-.205 1.239-.192.43-.543.899-1.053 1.408l-9.78 9.769z"
      />;
  });

let component = ReasonReact.statelessComponent("Logo");
let make =
    (
      ~text=true,
      ~className=?,
      ~rotate=?,
      ~size=?,
      ~id=?,
      ~animation=?,
      ~align=?,
      ~maximize=?,
      ~soloMax=?,
      ~contour=?,
      ~stroke=?,
      ~hover=?,
      ~animate=?,
      ~transparent=?,
      ~flipX=?,
      ~flipY=?,
      _children,
    ) => {
  {
    ...component,
    render: _self =>
      <IconBase
        name="logo"
        ?className
        ?animation
        ?align
        ?rotate
        ?size
        ?id
        viewBox="0 0 256 189"
        ?maximize
        ?soloMax
        ?contour
        ?stroke
        ?hover
        ?animate
        ?transparent
        ?flipX
        ?flipY>
        {Carbon.render(text)}
        {Crystal.render(text)}
      </IconBase>,
  };
};
