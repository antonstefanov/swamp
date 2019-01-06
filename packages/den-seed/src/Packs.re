module JsInterop_ = {
  module Hooks = {
    [@bs.deriving abstract]
    type t = {
      [@bs.optional]
      add: string,
      [@bs.optional]
      update: string,
      [@bs.optional]
      remove: string,
    };
    let make = t;
  };
  module Input = {
    [@bs.deriving abstract]
    type t = {
      description: string,
      [@bs.optional]
      readme: string,
      tags: array(string),
      [@bs.optional]
      hooks: Hooks.t,
    };
    external ofJson: Js.Json.t => t = "%identity";
  };
};
module Json = {
  [@bs.deriving abstract]
  type t = {
    title: string,
    description: string,
    tags: array(string),
    images: array(string),
    readme: string,
    [@bs.optional]
    hooks: JsInterop_.Hooks.t,
    created: float,
    modified: float,
  };
  let make = t;
  external ofJson: Js.Json.t => t = "%identity";
};

module JsInterop = {
  include JsInterop_;
  module Output = {
    [@bs.deriving abstract]
    type t = {packs: array(Json.t)};
    let make = t;
  };
};

module Pack = {
  module Hooks = {
    type t = {
      add: option(string),
      update: option(string),
      remove: option(string),
    };
    let make = (hooks: JsInterop.Hooks.t) => {
      add: hooks->JsInterop.Hooks.addGet,
      update: hooks->JsInterop.Hooks.updateGet,
      remove: hooks->JsInterop.Hooks.removeGet,
    };
    let toJson = (i: t) =>
      JsInterop.Hooks.make(
        ~add=?i.add,
        ~update=?i.update,
        ~remove=?i.remove,
        (),
      );
  };

  type t = {
    id: string,
    title: string,
    description: string,
    tags: array(string),
    images: array(string),
    readme: string,
    hooks: option(Hooks.t),
    created: float,
    modified: float,
  };

  let make = (c: Json.t): t =>
    Json.{
      id: c->titleGet,
      title: c->titleGet,
      description: c->descriptionGet,
      tags: c->tagsGet,
      images: c->imagesGet,
      readme: c->readmeGet,
      hooks: Belt.Option.map(c->hooksGet, Hooks.make),
      created: c->createdGet,
      modified: c->modifiedGet,
    };

  let toJson = (c: t) =>
    Json.make(
      ~title=c.title,
      ~description=c.description,
      ~tags=c.tags,
      ~images=c.images,
      ~readme=c.readme,
      ~hooks=?Belt.Option.map(c.hooks, Hooks.toJson),
      ~created=c.created,
      ~modified=c.modified,
      (),
    );
};

type t = {packs: array(Pack.t)};

let ofPacks = packs => packs |> Array.map(p => Pack.make(p));

let make = (j: JsInterop.Output.t): t =>
  JsInterop.Output.{packs: j |> packsGet |> ofPacks};

let toJson = (packs: array(Pack.t)): array(Json.t) =>
  packs |> Array.map(p => Pack.toJson(p));
