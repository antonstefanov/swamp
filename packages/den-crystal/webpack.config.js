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
  // not used
  devServer: {
    contentBase: [path.resolve(__dirname, "./src/public")],
    historyApiFallback: true,
    stats: {
      modules: false,
      entries: false,
    },
  },
  resolve: {
    alias: {
      styles: path.resolve(__dirname, "src/public/static/styles"),
      static: path.resolve(__dirname, "src/public/static"),
      src: path.resolve(__dirname, "src/"),
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
