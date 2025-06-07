# About

The Morrowind Script Extender makes additional functions available for use in Morrowind scripts, beyond those provided by the game's construction set.

These extra functions allow modders to add features and behavior to scripts that would otherwise be impractical or impossible.

!!! info "Morrowind Modding Community Discord"
	If you are looking for MWSE scripting advice or discussions, try the [Morrowind Modding Community Discord](https://discord.gg/QDEBbaP).

## Installation

Before running Morrowind with MWSE enabled, download these prerequisites:

- [Visual Studio x86 2022 Redistributable](https://aka.ms/vs/17/release/vc_redist.x86.exe). Note that you will need the x86 version, even if you already have the x64 redistributable installed.
- [Morrowind Code Patch](https://www.nexusmods.com/morrowind/mods/19510/) modifies the game executable, and MWSE assumes you have at least the base patches performed for its own modifications.
- [Morrowind Graphic Extender XE](https://www.nexusmods.com/morrowind/mods/41102) (MGE XE) allows MWSE to be loaded, and provides additional functionality.

Once these are installed, run MWSE-Update.exe in your Morrowind install folder. MGE XE comes bundled with MWSE's updater to make this process simpler. This updater downloads the [latest automated build from GitHub](https://github.com/MWSE/MWSE/releases/tag/build-automatic). These can be downloaded manually if the updater fails.


## Contributing

The Morrowind Script Extender is open source software that welcomes contributions in the forms of ideas, bug fixes, documentation, and new features. There is a separate guide for documentation contributions found [here](https://github.com/MWSE/MWSE/tree/master/docs).

### Requirements

#### Visual Studio

Contributing to MWSE requires a copy of [Visual Studio 2022](https://www.visualstudio.com/downloads/), with the following features enabled:

- Workloads:
	- Desktop development with C++
- Individual Components:
	- C++ MFC (only required for Construction Set Extender development)

#### LuaJIT

[LuaJIT](https://luajit.org/) needs to be compiled once, and is included in the project. To compile, use **Tools > Visual Studio Command Prompt** and run the following:

```bat
cd deps\LuaJIT\src
msvcbuild.bat
```

### Configuration

Project configuration is done via a .props file. To specify your own Morrowind installation directory, copy the SharedSE/MWSE.props file and rename the copy to SharedSE/MWSE.User.props. Use the Property Manager's User Macros page or directly edit the file to specify the location that Visual Studio should copy DLLs/PDBs to and where it should do its debugging. The directory is expected to have a trailing slash.


### GitHub

Code contributions are managed via [GitHub](https://github.com/MWSE/MWSE).


### Discord

Communication of features, designs, and requests are handled on Discord. See the #mwse channel in the [Morrowind Modding Community Discord](https://discord.gg/QDEBbaP).


## Authors

The following individuals have contributed to the Morrowind Script Extender.

- Anthony Garcia
- [C3pa](https://github.com/C3pa)
- Charles Cooley (cdcooley)
- Cody Erekson (Fliggerty)
- FreshFish
- [Galrom](https://github.com/Galrom-p)
- [GOOGLEPOX](https://github.com/googlepox)
- Grant McDorman
- [Greatness7](https://github.com/Greatness7)
- [Hrnchamd](https://github.com/Hrnchamd)
- [Merzasphor](https://github.com/Merzasphor)
- [Michael Wallar (NullCascade)](https://github.com/NullCascade)
- [OperatorJack](https://github.com/OperatorJack)
- [Pete Goodfellow (Petethegoat)](https://github.com/Petethegoat)
- [Pierre Steeg](https://github.com/pierresteeg)
- [Sebastien Levy (MetaBarj0)](https://github.com/MetaBarj0)
- Tim Peters
- Timeslip
