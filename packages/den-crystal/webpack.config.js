const path = require("path");

const isProd = process.env.NODE_ENV === "production";

module.exports = {
  entry: "./src/Index.bs.js",
  mode: isProd ? "production" : "development",
  output: {
    filename: "[name].js",
    path: path.join(__dirname, "./dist/build"),
    publicPath: "/build/",
  },
  devServer: {
    contentBase: [path.resolve(__dirname, "./src/static")],
    historyApiFallback: true,
    stats: {
      modules: false,
      entries: false,
    },
  },
  resolve: {
    alias: {
      src: path.resolve(__dirname, "src/"),
      static: path.resolve(__dirname, "src/static"),
    },
  },
  module: {
    rules: [
      {
        test: /\.md$/,
        use: [
          // {
          //     loader: "html-loader"
          // },
          {
            loader: "markdown-loader",
            options: {
              sanitize: true,
            },
          },
        ],
      },
      {
        test: /\.scss$/,
        use: [
          {
            loader: "style-loader",
          },
          {
            loader: "css-loader",
            options: {
              sourceMap: true,
            },
          },
          {
            loader: "sass-loader",
            options: {
              includePaths: ["./src/static", "./node_modules"],
              implementation: require("sass"),
              sourceMap: true,
            },
          },
        ],
      },
    ],
  },
};
