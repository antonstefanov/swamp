/* require css file for side effect only */
[@bs.val] external requireCSS: string => unit = "require";
/* require markdown */
[@bs.val] external requireMD: string => unit = "require";
/* require an asset (eg. an image) and return exported string value (image URI) */
[@bs.val] external requireAssetURI: string => string = "require";

[@bs.val] external env_public_url: string = "process.env.PUBLIC_URL";
