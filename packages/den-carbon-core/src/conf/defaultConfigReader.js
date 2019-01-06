const config = require("./config.json");

let userConfig = require("./user-config.json");

// you can override index.js with custom merge logic
const mergedConfig = {
  ...config,
  ...userConfig,
};

const getConfig = () => mergedConfig;

exports.default = getConfig;
