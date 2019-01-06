# carbon

## structure

`packs_core` - the default, installed packs (commands)
`packs` - packs available for installation

## new packs

To create a new pack/command in `reason`:

- create a new file
- `cmdsync`->`tab` -> a snippet with all required fields will be populated

Each pack requires:

- a folder - this will be the name of the command
- a `cmd.jsonc` file - these are validated with a schema and autocompletion is available
- either a `README.md` file or a `readme` field in `cmd.jsonc` with a remote url
- a 16:9 image in an `images` folder (the first one will be used for the card image)
