import resolve from "rollup-plugin-node-resolve";
import commonjs from "rollup-plugin-commonjs";
import json from "rollup-plugin-json";
import ignore from "rollup-plugin-ignore";

export default [
  {
    input: "src/main.js",
    output: { file: "dist/main.js", format: "cjs" },
    plugins: [
      ignore(["assert", "util"]),
      resolve({
        preferBuiltins: false,
      }),
      commonjs(),
      json(),
    ],
  },
];
