let bem_prefix = component_class => component_class ++ "-";
let bem_modifier = component_class => component_class ++ "--";
let bem_element = component_class => component_class ++ "__";

let element = (component_element, str) => component_element ++ str;
let appendix = (component_prefix, str) => component_prefix ++ str;
let modifier = (component_modifier, str) => component_modifier ++ str;
let modifierIfSome = (component_modifier, str) =>
  Cn.mapSome(modifier(component_modifier), str);

type bem = {
  className: string,
  appendix: string => string,
  appendixIfSome: option(string) => string,
  appendixIfTrue: (string, bool) => string,
  modifier: string => string,
  modifierIfSome: option(string) => string,
  modifierIfTrue: (string, bool) => string,
  element: string => string,
};

let make = component_class => {
  let component_modifier = bem_modifier(component_class);
  let component_prefix = bem_prefix(component_class);
  let component_element = bem_element(component_class);

  let compiledAppendix = appendix(component_prefix);
  let compiledModifier = modifier(component_modifier);
  {
    className: component_class,
    appendix: compiledAppendix,
    appendixIfSome: Cn.mapSome(compiledAppendix),
    appendixIfTrue: (str, isTrue) => isTrue ? compiledAppendix(str) : "",
    modifier: compiledModifier,
    modifierIfSome: Cn.mapSome(compiledModifier),
    modifierIfTrue: (str, isTrue) => isTrue ? compiledModifier(str) : "",
    element: element(component_element),
  };
};
