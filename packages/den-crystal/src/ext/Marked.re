module MarkedOptions = {
  [@bs.deriving abstract]
  type t = {
    [@bs.optional]
    sanitize: bool,
    /* baseUrl: null,
       breaks: false,
       gfm: true,
       headerIds: true,
       headerPrefix: '',
       highlight: null,
       langPrefix: 'language-',
       mangle: true,
       pedantic: false,
       renderer: new Renderer(),
       sanitize: false,
       sanitizer: null,
       silent: false,
       smartLists: false,
       smartypants: false,
       tables: true,
       xhtml: false */
  };
  let make = t;
};
[@bs.module "marked"]
external _marked:
  (
    string,
    MarkedOptions.t,
    (Js.Null_undefined.t(Js.Exn.t), string) => unit
  ) =>
  unit =
  "parse";

let m = (options, src, callback) => _marked(src, options, callback);

let parse =
    (~options=MarkedOptions.make(~sanitize=true, ()), src)
    : DenSeed.AsyncResult.promiseT(string) =>
  DenSeed.AsyncResult.make2(m(options), src);
