# MWSE Documentation

Looking for MWSE documentation? Please [click here](https://mwse.github.io/MWSE/).


## Synopsis

The docs for this project are created for [GitHub Pages](https://pages.github.com/) using [mkdoc](https://www.mkdocs.org). Documentation is done in the Markdown format. Once a doc is pushed to this repository it will automatically be updated at [the documentation site](https://mwse.github.io/MWSE/).


## Autocomplete Definitions

Some portions of the docs, include Lua type information, events, and APIs, are auto-generated using the content in the autocomplete root folder. Edit these definitions instead, and run the configured vscode build task in the autocomplete folder to regenerate the needed files.

There are three basic types of definitions:
- [event definitions](https://github.com/MWSE/MWSE/blob/docs-contributions-guide/docs/event-definitions-guide.md)
- [type definitions](https://github.com/MWSE/MWSE/blob/docs-contributions-guide/docs/type-definitions-guide.md)
- [function definitions](https://github.com/MWSE/MWSE/blob/docs-contributions-guide/docs/function-definitions-guide.md)

Clicking on the links above will take you to a guide for writing mentioned definitions.

## Building

Documentation can also be built locally for testing or personal use. To do this you will need a copy of [Python](https://www.python.org/).

The following modules need to be installed:

```bat
pip install mkdocs mkdocs-material mkdocs-awesome-pages-plugin
```

Another build task is available in vscode to live test the docs locally.

## Developing Tips

When writing the examples, always use `tes3.*` constants when available. Also, consider suggesting when a certain function accepts values from `tes3.*` namespace. Note that some older areas of the documentation might not follow the conventions established in the guides above. Those can be updated if editing the affected definitions.

Some parts of the documentation are written by hand. Namely, those in:
- docs\source\references
- docs\source\guides

### Admonitions

The following admonition classes are available: `abstract`, `attention`, `bug`, `caution`, `check`, `cite`, `danger`, `done`, `error`, `example`, `fail`, `failure`, `faq`, `help`, `hint`, `important`, `info`, `missing`, `note`, `question`, `quote`, `success`, `summary`, `tip`, `tldr`, `todo`, `warning`.
