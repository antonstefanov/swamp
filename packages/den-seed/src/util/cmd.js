const { spawn } = require("child_process");
const kill = require("tree-kill");

const getLoggedCallback = (logger, level, prepend, fn) => (...args) => {
  logger[level](`${prepend}`, ...args);
  return fn(args);
};
class ExitError extends Error {
  constructor(code, signal, result, message) {
    super(message);
    this.name = "ExitError";
    this.code = code;
    this.signal = signal;
    this.result = result;
  }
}
class CmdBuffer {
  constructor(
    cmd,
    providedArgs,
    logger,
    options = {},
    providedCmdOptions = {},
  ) {
    this.handleStderr = this.handleStderr.bind(this);
    this.handleError = this.handleError.bind(this);
    this.handleData = this.handleData.bind(this);
    this.handleExit = this.handleExit.bind(this);
    this.kill = this.kill.bind(this);

    const { forwardOutput, ...cmdOptions } = providedCmdOptions;
    const args = forwardOutput
      ? [...providedArgs, "< /dev/tty", "> /dev/tty"]
      : providedArgs;

    const child = spawn(cmd, args, cmdOptions);

    logger.info(
      `CMD ${child.pid} -->`,
      `${cmd} ${providedArgs.join(" ")}`,
      cmdOptions,
    );

    this.options = options;
    this.logger = logger;
    this.result = "";
    this.errors = "";

    child.stderr.setEncoding("utf8");
    child.stdout.setEncoding("utf8");

    child.on("error", this.handleError);
    child.on("exit", this.handleExit);
    child.stderr.on("data", this.handleStderr);

    child.stdout.on(
      "data",
      getLoggedCallback(logger, "debug", "OUT", this.handleData),
    );
    if (options.onMessage) {
      child.on(
        "message",
        getLoggedCallback(logger, "debug", "MESSAGE", options.onMessage),
      );
    }

    this.child = child;
  }
  handleData(chunk) {
    this.result += chunk;
    if (this.options.onOutData) this.options.onOutData(chunk);
  }
  handleStderr(output) {
    this.logger.error("STDERR", output);
    this.hasErrors = true;
    this.errors += output;
    if (this.options.onErrorData) {
      return this.options.onErrorData(new Error(output));
    }
  }
  handleError(error) {
    this.hasErrors = true;
    this.kill();
    this.options.onError(error);
  }
  handleExit(code, signal) {
    const failed = code !== 0;
    if (failed) {
      this.logger.error(
        `CMD ${this.pid} <-- FAILED with code ${code}`,
        this.result,
      );
      return this.options.onError(
        new ExitError(
          code,
          signal,
          this.result,
          `Exited with code ${code}, signal ${signal}`,
        ),
      );
    } else {
      const level = this.hasErrors ? "warn" : "success";
      this.logger[level](
        this.hasErrors
          ? `CMD ${this.pid} <-- COMPLETED WITH ERRORS`
          : `CMD ${this.pid} <-- SUCCESS`,
        this.result,
      );
      return this.options.onCompleted({
        hasError: this.hasErrors,
        result: this.result,
        errors: this.errors,
      });
    }
  }
  kill(signal = undefined) {
    const pid = this.pid;
    this.child.stdin.destroy();
    try {
      kill(pid, signal);
    } catch (err) {
      this.logger.error(`Kill error pid=${pid}`, err);
    }
  }
  get pid() {
    return this.child.pid;
  }
}

const noop = () => {};
const runAsync = (
  logger,
  cmd,
  args,
  { timeout = 10000, ...options } = {
    onOutData: noop,
    onErrorData: noop,
    onMessage: noop,
  },
  cmdOptions = {},
) =>
  Promise.race([
    new Promise((_resolve, reject) =>
      setTimeout(reject, timeout, new Error("Command timed out")),
    ),
    new Promise((resolve, reject) => {
      try {
        new CmdBuffer(
          cmd,
          args,
          logger,
          {
            ...options,
            onCompleted: resolve,
            onError: reject,
          },
          cmdOptions,
        );
      } catch (e) {
        reject(e);
      }
    }),
  ]);

module.exports = {
  runAsync,
};
