# MWSE Documentation

Looking for MWSE documentation? Please [click here](https://mwse.readthedocs.io/).


## Synopsis

The docs for this project are created for [Read the Docs](https://readthedocs.org/) using [Sphinx](http://www.sphinx-doc.org/). Documentation is done in the [reStructuredText](http://docutils.sourceforge.net/rst.html) format. Once a doc is pushed to this repository it will automatically be updated at [the documentation site](https://mwse.readthedocs.io/).


## Autocomplete Definitions

Some portions of the docs, include Lua type information, events, and APIs, are auto-generated using the content in the autocomplete root folder. Edit these definitions instead, and run the `autocomplete\readthedocs\build.lua` file to regenerate the needed files.


## Building

Documentation can also be built locally for testing or personal use. To do this you will need a copy of [Python](https://www.python.org/).

The following modules need to be installed:

```bat
pip install sphinx sphinx-autobuild sphinx_rtd_theme
```

Once this is done, docs can be built using:

```bat
cd docs
.\make.bat html
```

The created files will be found in the `docs\build\html` folder.
