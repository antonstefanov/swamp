import resolve from "rollup-plugin-node-resolve";
import commonjs from "rollup-plugin-commonjs";
import json from "rollup-plugin-json";

const FAKE_FILE = "export default {}";
const IGNORE = "\0empty_module";

const proxy = packName => `\u0000commonjs-proxy:${packName}`;
const ignore = packs => {
  const packsSet = new Set(packs);
  return {
    resolveId: packName => (packsSet.has(packName) ? IGNORE : null),
    load: id => (id === IGNORE ? FAKE_FILE : null),
  };
};

export default [
  {
    input: "src/main.js",
    output: { file: "dist/main.js", format: "cjs" },
    external: ["assert", "util", "yargs", "path", "os", "fs", "child_process"],
    plugins: [
      ignore(["./user-config.json"]),
      resolve({
        preferBuiltins: false,
      }),
      commonjs(),
      json(),
    ],
  },
];
