# Metadata

A metadata file is a file that contains information about your mod, such as its name, version, and dependencies on other mods or tools. This information is used by MWSE to ensure that your mod runs correctly and to notify users if any required files or tools are missing. It also has the potential be used by external tools such as mod packagers.

It is highly recommended you provide a metadata file for your mod. Not only will it allow users of your mod to be notified if any dependencies are missing, but it will also allow other mods to easily add your mod as a dependency as well.

The following sections will describe how to make a metadata file for your mod. A full example is provided at the end.

## Creating a Metadata File

First, create a new file in the `Data Files` directory called `"<Mod Name>-metadata.toml"` (for example, the metadata for the mod Ashfall would be called `"Ashfall-metadata.toml"`). While not necessary, it is recommended that the name of your metadata file matches the name of your `.ESP` file, if your mod has one.

Now open your new file and enter the following sections.

!!! note
	*Metadata files are written in the TOML configuration file format. It is highly recommended you use an IDE or text editor with TOML syntax highlighting, such as [VS Code](https://code.visualstudio.com/) or [notepad++](https://notepad-plus-plus.org/) to ensure your file is valid. For more information on the TOML syntax, see the [TOML website](https://toml.io/en/).*

## Package Section

The `[package]` section of the metadata file is where you should fill in information about your mod.

The following fields are available:

* `name` - The name of your mod.
* `description` - A short description of your mod.
* `homepage` - The URL of your mod's homepage, such as the NexusMods download page.
* `repository` - The URL of your mod's repository (e.g github) if you have one.
* `authors` - A list of the authors of your mod.
* `version` - The version of your mod, used for dependency checking

At a minimum you should provide a `name` and `version`, and probably a `homepage`.

Example:

```toml
    [package]
    name = "My Mod"
    description = "A description of the mod"
    homepage = "https://www.nexusmods.com/morrowind/mods/49658"
    repository = "https://github.com/jhaakma/skoomaesthesia"
    authors = ["Merlord", "Greatness7"]
    version = "7.8.9"
```

## Tools Section

The `[tools]` section is for tool-specific configuration.

The following fields are available:

* `mwse`

    Here you can specify MWSE-specific information about your mod.

    The following fields are available:

    * `lua-mod` - The path to the main.lua associated with this mod. For example, if your main.lua file is located in `"Data Files\MWSE\mods\g7\myMod\main.lua"`, then this field should be set to `"g7.myMod"`.
    * `load-priority` - The priority for when this mod is loaded. Higher numbers are loaded first.
    * `wait-until-initialize` - Whether to wait until the game has initialized before loading this mod.

    Example:
    ```toml
        [tools.mwse]
        lua-mod = "mer.myMod"
        load-priority = 100
        wait-until-initialize = true
    ```

* `csse`

    Here you can specify a list of deprecated object IDs. Deprecated objects aren't listed in the Object Window in the Construction Set, but are still present in the master/plugin file. These are mainly useful to resource mods. The ID list is case-insensitive.

    Examples:
    ```toml
        [tools.csse]
        deprecated = [
            "bm nordic silver longsword_cft",
            "BM nordic silver longsword_ber",
            "BM ice longsword_FG_Unique"
        ]
    ```



## Dependencies Section

The `[dependencies]` section of the metadata file is where you should list any dependencies your mod has. Dependencies are checked before your `main.lua` file is executed and will warn the player if any are missing.

The following dependencies are available:

-  `assets`

    A list of assets that your mod requires, such as sounds, textures and meshes. Assets can be directories or loose files. The path is relative to the `Data Files` directory.

    Example:

    ```toml
        [dependencies]
        assets = [
            "Sound\\mySounds",
            "Textures\\tex.dds",
            "Meshes\\mesh.nif"
        ]
    ```


-  `archives`

    A list of archives (e.g BSA files) required by your mod and the assets they contain. This dependency will only fail if the BSA is inactive and the assets are missing. If the BSA has been unpacked manually, the assets will be found and the dependency will pass.

    Example:

    ```toml
        [dependencies.archives]
        "TR_Data.bsa" = [
            "Textures\\TR"
        ]
    ```

-  `mwse`

    A dependency on a minimum version of MWSE. This is especially useful if you are using any recently added MWSE features.

    The version is checked using the build number, which is the number that comes after "v2.1.0" in the first line of `MWSE.log`. E.g, the `2907` in the following line:

        Morrowind Script Extender v2.1.0-2907 (built Feb 18 2023) hooked.

    The buildnumber can also be checked using `mwse.buildNumber` in the lua console.

    Example:

    ```toml
        [dependencies.mwse]
        buildnumber = 2907
    ```

-  `mge-xe`

    A dependency the MGE XE version. This follows the same semver pattern as mod dependencies, so you should put `>=` before the version number to require a minimum version.

    The version is checked using the version string, which can be found in the second line of `MWSE.log` (ignore the last `.0`):

        Found MGE XE. Version: 0.15.0.0

    The version can also be checked using `mge.getVersion()` in the lua console.

    Example:

    ```toml
        [dependencies.mge-xe]
        version = ">=0.15.0"
    ```

-  `mods`

    A list of mods required by your mod. This can be used to check if a plugin (esp or esm) is active, or if a lua mod is installed.

    The following fields are available:

    -  `plugin`

        The name of the plugin (esp or esm) to check for.

        Example:

        ```toml
            [dependencies.mods."OAAB Data"]
            plugin = "OAAB_Data.esm"
        ```

    -  `mwse-module`

        The name of the lua mod to check for. This can be a file in `Data Files\MWSE\mods\` or `Data Files\MWSE\lib` etc, using the mwse module loading syntax (i.e, the string you would pass to `include` or `require` to load the mod).

        Example:

        ```toml
            [dependencies.mods."My Lua mod"]
            mwse-module = "mer.myLuaMod"
        ```

    -  `version`

        The minimum version of the mod required. This is checked using the version field in the mod's metadata file.

        The key must match the metadata file name. For example, in the below example, the key `"My Other Mod"` must match the metadata file `"Data Files\My Other Mod-metadata.toml"`.

        See the `Versions` section below for more information about versioning.

        Example:

        ```toml
            [dependencies.mods."My Other Mod"]
            version = "^7.8.9"
        ```

	- `url`

		The url to download the missing/outdated mod dependency. If provided and this dependency fails, a button will appear in the failed dependency pop-up which will close the game and launch this URL in their browser.

		Example:

		```toml
		    [dependencies.mods."My Other Mod"]
			mwse-module = "mer.myOtherMod"
			version = ">=7.8.9"
			url = "https://www.nexusmods.com/morrowind/mods/52116"

		```

-  `mcp`

	A dependency on specific Morrowind Code Patch features. Pass a list of features and the needed state for each feature: enabled (true) or disabled (false). You can see the [list of MCP features here](https://mwse.github.io/MWSE/references/code-patch-features/). The descriptions for each MCP feature are found in the `mcpatch\\describe.json` file. Pass the feature key from the `tes3.codePatchFeature` table. For example, `"creatureVoiceoverEnable = true"`.

	Example:

	```toml
		[dependencies.mcp.features]
		# The following MCP feature needs to be enabled
		creatureVoiceoverEnable = true
		# The mod is uncompatible with the following MCP feature,
		# it needs to be disabled for the mod to work properly
		arrowDenocker = false
	```

## Versions

The version strings in `[package]`, `[dependencies.mge-xe]`, and `[dependencies.mods]` follow the semver (Semantic Versioning) format, which consists of three numbers separated by periods. The syntax is `<MAJOR>`.`<MINOR>`.`<PATCH>`. For example, `1.2.3` means the major version is 1, the minor version is 2, and the patch version is 3. You should increment the major version when you make a breaking change, the minor version when you add new features, and the patch version when you make bug fixes.

For more information on semver, see the [semver website](https://semver.org/).

### Comparison Operators

When specifying a version for a mod or mge-xe dependency, use a comparison operators to specify a minimum version.

Available dependency operators are: `^`, `=`, `>=`, `<=`, `>`, `<`.

The  `=`, `>=`, `<=`, `>`, and `<` operators all function as expected. The pessemistic operator (`^`) works by using `=` on major versions and `<=` on minor and patch versions. For example, if your target version is `^1.2.3`, then `1.2.3`, `1.3.3`, and `1.2.4` will be compatible, but `2.0.0`, `1.1.3`, and `1.2.2` will not. The idea behind the `^` operator is that changes in major versions signify breaking changes, which can introduce incompatibilities.

## Full Example

```toml
# Data Files/mymod-metadata.toml

# Holds basic information about the mod
[package]
name = "My Mod"
description = "A description of the mod"
homepage = "https://www.nexusmods.com/morrowind/mods/49658"
repository = "https://github.com/jhaakma/skoomaesthesia"
authors = ["Merlord", "Greatness7"]
version = "7.8.9"

# MWSE specific information about this mod
[tools.mwse]
lua-mod = "mer.myMod"
load-priority = 100
wait-until-initialized = true

# Dependencies are checked on `initialized` and warn the player if any are missing
[dependencies]
  # Assets can be directories or loose folders
  assets = [
      "Sound\\mySounds",
      "Textures\\tex.dds",
      "Meshes\\mesh.nif"
  ]

# These will only throw an error if the assets are missing AND the BSA is inactive
[dependencies.archives]
"TR_Data.bsa" = [
    "Textures\\TR"
]

# Check MWSE version using build number (the number that comes after "v2.1.0" in MWSE.log)
[dependencies.mwse]
buildnumber = 2907

# Check MGE XE version
[dependencies.mge-xe]
version = ">=0.15.0"

# mwse-module can be a folder or lua file, anywhere in /mods, /lib etc
[dependencies.mods."My Lua mod"]
mwse-module = "mer.myLuaMod"

# Check if a plugin (esp or esm) is active
[dependencies.mods."OAAB Data"]
plugin = "OAAB_Data.esm"

# Finds the version from the metadata file of the dependency
# If url is provided, a download button will appear if the dependency isn't met
[dependencies.mods."My Other Mod"]
version = ">=7.8.9"
url = "https://www.nexusmods.com/morrowind/mods/52116"

# Check if user has specific MCP feature enabled or disabled
[dependencies.mcp.features]
# The following MCP feature needs to be enabled
creatureVoiceoverEnable = true
# The mod is uncompatible with the following MCP feature,
# it needs to be disabled for the mod to work properly
arrowDenocker = false
```
