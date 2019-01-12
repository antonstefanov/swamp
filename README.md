# install

```sh
yarn global add den-carbon
```

## crystal commands

start `crystal` dev server (listening on :8080)

```sh
o start
```

Update the displayed carbon packs after making changes

```sh
o refresh
```

---

## pack commands

list all installed packs

```sh
o ls
```

add a pack

```
o add name
```

update a pack (or leave name empty to update all)

```
o update name
```

remove a pack

```
o remove name
```

---

# dev

Run

```sh
yarn first
```

in `swamp` folder to:

- install/update compiler
- install dependencies
- build
- initialise the global CLI
- optionally add CLI command autocompletion

to run webpack bundler

```sh
yarn start-crystal
```

to start static server that serves page (usually used in combination with `yarn start-crystal`)

```sh
yarn start-server
```

---

## seed

Shared data types and core library.

## carbon

CLI orchestrator. All commands are run with `o`, help is available with `--help` or `-h`.

## crystal

UI for viewing and searching for available packs.
