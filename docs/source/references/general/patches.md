# Patches

The following patches are included by MWSE.

!!! tip The Construction Set Extender

	This project also improves the Construction Set. Read more about changes to that program on the [CSSE](https://mwse.github.io/MWSE/references/general/csse/) page.

## Features

* Allows custom classes to have custom images on the class selection menu. The texture used is a bmp file found in the textures\\levelup directory, whose name is the id of the class.
- Allows Morrowind to run when not in focus.
- Allows movies to be played with letterboxing.
- Allows NiFlipController to specify its affected map correctly.
- Allows NiLinesData to be loaded from meshes.
- Allows NiUVController to specify its texture correctly.
- Allows NiTriShapes to optionally utilize software skinning, which can use more than 4 bones per drawable, by setting flags 0x200 in the NIF.
- Creates crash dump files and logging information to help diagnose mod issues.
- `Disable` mwscript function updates collision information so the player won't collide with invisible references.
- Improved support for NiSortAdjustNode, with additional alpha subtree rendering.
- Improves load times when using Mod Organizer 2.
- Optimizes access to global variables.
- Optimizes access to player kill count, via `GetDeadCount` or dialogue filtering.
- Optimizes disposition calculation when dialogue filtering.
- Optimizes `DontThreadLoad` ini file access to not always fetch from the file system.
- Optimizes journal updating.
- Optimizes `ShowMap` and `FillMap` to not be absurdly slow.
- Raises mod limit from 256 to 1024.
- Replaces Morrowind's dialogue filtering system with one that is much more performant.
- Fixes the activation raytest when around skinned objects, such as creatures/NPCs.


## Bug Fixes

- Allows the game to correctly close when quit with a messagebox popup, preventing rogue Morrowind.exe processes from hanging in the background.
- Athletics and sneak skill progress code made consistent with other skill advancements. Useful to modders.
- Correctly initialize MobileProjectile tag/objectType. Useful to modders.
- `Enable` and `Disable` mwscript functions no longer can cause crashes with script variables unset.
- Fixes book and weapon enchantment copying. Useful to modders.
- Fixes cloning of NiObjects with multiple NiStringExtraData attachments.
- Fixes crash when cloning certain NiSortAdjustNode structures.
- Fixes crash when releasing a clone of a light with no reference.
- Fixes crash when saving active VFXs when none are serializable.
- Fixes crash when saving menu position if the derived key name is too long.
- Fixes crash when updating cell markers at the border of the drawable map area.
- Fixes crash where NPC flee logic tries to select a random node from pathgrids with 0 nodes.
- Fixes crash with paper doll equipping/unequipping.
- Fixes crash with uncloned actors removing items.
- Fixes issue where VFX objects would load incorrectly, or cause save bloat.
- Fixes sound volume calculations. Additional fixes provided when sound loops.
- Fixes terrain render issues when using more than 500 land textures.
- Fixes time being nudged forward by small extra increments when resting.
- Fixes time being truncated when advancing time past midnight.
- Fixes transparency effects from invisibility/chameleon from being desynced when changing equipment.
- Prevents empty menu positions from saving to the ini file.
- Updates animations for third person and first person player reference when idle mode is flagged.
- Symbolically linked files now list and load correctly.
