import resolve from "rollup-plugin-node-resolve";
import commonjs from "rollup-plugin-commonjs";
import json from "rollup-plugin-json";

export default [
  {
    input: "src/main.js",
    output: { file: "dist/main.js", format: "cjs" },
    external: ["assert", "util", "yargs", "path", "os", "child_process"],
    plugins: [
      resolve({
        preferBuiltins: false,
      }),
      commonjs(),
      json(),
    ],
  },
];
