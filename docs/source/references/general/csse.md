# Construction Set Extender

The Construction Set Extender, also known as CSSE, is a project that aims to improve the functionality of the TES Construction Set. This page serves to document the project.

To request a feature, visit the [Construction Set Extender Development issue](https://github.com/MWSE/MWSE/issues/397) on GitHub. Support for the project can be found in the #mwse channel in the [Morrowind Modding Community](https://discord.me/page/mwmods).

For a list of recent changes, [visit the CSSE project folder commit history](https://github.com/MWSE/MWSE/commits/master/CSSE).

!!! tip The Morrowind Script Extender

	This project also improves Morrowind's execution. Read more about fixes and features for the core game on the [patches](https://mwse.github.io/MWSE/references/general/patches/) page.

## Requirements

CSSE requires [MGE XE](https://www.nexusmods.com/morrowind/mods/41102?) v0.14.3 (released July 29, 2022) or later in order to be automatically loaded into the Construction Set.

If using OpenMW, you will still need to install MGE XE into your Morrowind installation directory (the same directory that you launch the Construction Set).

If you are on Linux, you also need additional setup in winecfg so that WINE will use the included dlls. Read [Hristos' guide](https://hristos.co/blog/enjoying-morrowind-on-linux-part-one-original-engine/) for a full walkthrough. Specifically, in winecfg, the dlls d3d8, mwse, csse need to be set to native and builtin.

## Installation

You can obtain the CSSE using one of the following methods:

### MWSE-Updater

MGE XE includes the MWSE-Update program. This program will download the latest CSSE file (as well as the latest MWSE) into your Morrowind installation directory.

To get CSSE with this method, simply run the program and follow any prompts that may be displayed (such as closing the Construction Set). It may only open briefly, in which case you are likely already up-to-date. The updater won't download the file if you already have the latest version.

### Manually

The same download can be found on the [Automated Build release on GitHub](https://github.com/MWSE/MWSE/releases/tag/build-automatic). You can download the mwse.zip file, which contains both MWSE and CSSE.

Once you have the zip file downloaded, extract its contents to the Morrowind installation directory. Both CSSE.dll and TES Construction Set.exe must reside in the same directory.

If you are strictly an OpenMW user but also a TES Construction Set user, you need only extract the CSSE.dll file into your Morrowind directory. However, for testing in the vanilla engine you should still have the [Morrowind Code Patch](https://www.nexusmods.com/morrowind/mods/19510) and MWSE installed. MWSE offers various [patches and other features](https://mwse.github.io/MWSE/references/general/patches/) to help make the original engine faster and more stable, in the same way that CSSE does for the Construction Set.

## Compatibility

If you are using [Construction Set Better UI](https://www.nexusmods.com/morrowind/mods/50311), CSSE will not automatically load. To get CSSE to work with Better UI, you will need to rename the Better UI executable to "TES Construction Set" and replace the existing vanilla CS file. Be sure to back the original up first!

## Features

Prefer to learn from videos? Danae has made [a non-exhaustive summary video of the CSSE](https://www.youtube.com/watch?v=uGOA7MIKsw0), last updated May 5th, 2023.

### General

* General optimizations to UI rendering. Status updates are throttled to only update once every 20ms, which drastically improves load times.
* The following warnings will no longer or only conditionally create popup windows, but will still be written to Warnings.txt:
	* "Following string is different for topic" and "Previous string is different for topic" are suppressed entirely.
	* The "1 duplicate references were removed" from loading Tribunal and Bloodmoon will no longer make a popup. If this is not the vanilla duplicate reference, or if there is more than one duplicate reference, a popup will still be shown.
* Fixed GMST pollution bug caused when not loading Tribunal and Bloodmoon.
* A QuickStart feature. In the render window, use the context menu (Q) to save the currently loaded plugins, the active plugin, current cell, camera position, camera orientation. Whenever the CS is loaded with no other plugin loading, it will automatically load all the saved plugins, and load the cell with the viewport saved. This can be cleared or disabled. This can be done with no active loaded cell if you do not want to automatically load a cell on startup.
* The CS will no longer adopt a working directory externally. This means that when opening esm/esp files from the Data Files folder from double clicking it, the Morrowind.ini file and all other configs/logs will correctly load from the root Morrowind folder.
* Loading an active file without manually loading its masters will no longer lead to a potential crash.
* Opening the Construction Set through Mod Organizer 2 is no longer slowed by the number of active MO2 mod folders.
* The NetImmerse NiLinesData structure loads correctly.
* Symbolically linked files now list and load correctly.
* By default both dds and tga texture files are selectable in texture selection dialogs.
* Attempts to open the CS .hlp file now instead redirect to the [Morrowind Modding Wiki mirror](https://tes3cs.pages.dev/) of the document.

#### Testing from the Construction Set

Both Morrowind and OpenMW can be launched from the CS with a specified environment. Two new buttons have been added to the toolbar, the first with Morrowind's icon, and the second with OpenMW's icon.

When one of these buttons is clicked, the appropriate game launches with only the active plugins loaded in the Construction Set loading. Instead of seeing the main menu, you will be loaded to a specified position with a mostly fresh character.

To change the starting location, use the Render Window's quick menu. You can click on a reference and test at its position (e.g. where a door marker is, or on a carpet) or at the camera position.

##### OpenMW Support

The OpenMW icon will only display if a valid OpenMW installation directory has been provided. This must be entered into the csse.toml file:

```toml
[openmw]
location = "D:\\Games\\OpenMW"
```

Because of the implementation of the OpenMW support, a custom config directory is used at "Data Files\MWSE\tmp\csse\openmw". CSSE will make a copy of your settings.cfg file in this folder, but will not keep it synced to any future changes in the OpenMW install.

##### Test Environment

Developers can customize the starting environment of the test character to test changes in quests. This can be done by editing the csse.toml file to specify items, spells, topics, quest states, and global variable values they want to use. Below is an example environment that gives the player a relatively fresh start with some experience in the Mages' Guild:

```toml
[test_environment]
spells = ["fireball","mark","recall","levitate"]
topics = ["duties", "advancement", "latest rumors", "little secret"]

[test_environment.inventory]
"iron dagger" = 1
ingred_gold_kanet_01 = 3
ingred_stoneflower_petals_01 = 4
ingred_willow_anther_01 = 2
ingred_heather_01 = 4
gold_001 = 5000

[test_environment.journal]
MG_Flowers = 10
MG_Sabotage = 100
MG_BCShrooms = 100

[test_environment.globals]
chargenstate = -1.0
```

### Main Window

* The status bars at the bottom of the construction set have been widened. They should always have enough room to show the relevent information.
* The status bars now also show the scale of the selected object.

### Actor Editing Windows

* The AI window opens more quickly.
* Clicking on the return button in the AI window when no cell has been loaded no longer crashes the CS.

### Cell View

* Significantly improved UI responsiveness.
* Fixed issue where the cell and object lists would gradually shrink when data files are reloaded.

### Dialogue Window

* Optimized UI. Changing between INFOs should be nearly instantaneous. Initial dialogue load time reduced by 60%.
* Condition variable lists will now always be wide enough to fit the convents. Journals, locals, and items will no longer have their IDs cut off.
* Modified topics/INFOs are highlighted green. Deleted topics/INFOs are highlighted red.
* The window can be filtered to only show modified data.
* The window can now be resized and maximized.
* Layout changes:
	* The sex condition is now under the race condition, instead of above the function/variable list. It is not hidden (but is disabled) when viewing journal dialogues.
	* The journal conditions (name/finished/restart) are shown but disabled for other dialogue types.
	* The general UI has minor other position layout changes.
* Select all control (Ctrl+A) now functions in the edit boxes.
* The window will remember the user preferences for column widths, as well as the overall window size.
* Conditional columns display the value being compared against to make it easier at a glance what the INFO requires.
* The cell filtering behavior can be changed using a new dropdown under the actor filter dropdown.
	* Filter with first reference's cell: The first reference of an actor's cell will be used when parsing Cell Fun/Var checks. This is the vanilla and default construction set behavior.
	* Filter with render window cell: The currently open cell in the render window will be used instead of the reference's current cell when filtering dialogue.
	* Ignore cell filter: Cell filtering will be ignored completely; any cell will assume to pass.
* When filtering by an actor and choosing local variables to filter by, only locals already defined on that actor's script will show in the dropdown list.

### Object Window

* Significantly improved UI responsiveness.
* The tab style has been changed to be more button-like. This prevents tabs from moving around.
* Added a footer to the window, with a new filter input. This input can be focused by using CTRL+F while the Object Window is active. This can be used to filter objects by a matching id, name, icon, model, enchantment, script, or book text fragments.
	* Regex search options and case-sensitive search options can be enabled in the config file.
* The footer also supports a toggle button to only show modified objects.
* The data rendering and behavior has been completely rewritten. Strings and alignment is more consistent. New columns can be added.
	* A new modified column is displayed, showing if the object is modified. This can be sorted on.
* Fixed issue where box selection in the object window would cause the mouse to snap to the edge of the screen.
* Modified objects are highlighted with a light green background.
* Deleted objects are highlighted with a light red background.
* The spell list lists the range of the spell.
* The creature list now shows the soul value of creatures.
* The leveled item/leveled creature lists now show the chance of no spawn.
* Objects can be marked as deprecated in a content file's metadata file. An array of IDs can be provided under `tools.csse.deprecated`. Objects matching these IDs (case insensitive) will not be displayed in the Object Window.

### Path Grid Window

* The default focused control is now the save button, instead of the Generate Default Grid button.

### Reference Edit Window

* Opening the window now no longer dirties the reference, until a value has been actually changed.

### Render Window

* Clicking or otherwise interacting with skinned objects, such as NPCs, no longer consumes the mouse. It is now easier to click on objects near actors.
* Reference rotation is now performed on the world axis, rather than the local axis. To rotate on the local axis as it worked in vanilla, hold the alt key.
* Dropping a reference (using the F key) is more reliable and accurate. It works on skinned references as well.
* References can be group-scaled. By holding the alt key when scaling, all selected references will scale together and reposition accordingly.
* Reference moving will follow the cursor, instead of using the movement scale. This can be disabled in the settings window.
* Camera panning will anchor to be relative to the object under the cursor, instead of moving at a fixed rate. This can be disabled in the settings window.
* References can be moved onto surfaces. By holding the alt key when drag-moving a reference, it will rotate and snap onto the surface under the cursor. The context menu (Q) can be used to set the axis to snap to.
* Extended editor marker support. Toggling markers (M) will now also toggle markers supported by the game executable. This allows custom markers to be toggled by tagging them with a MRK string and adding a EditorMarker node, or by defining objects whose name begins with or matches "Tri EditorMarker".
* A new context menu is available, using the Q key. From it you can access some new features:
	* Set the snap axis when alt-drag-moving references.
	* Toggle between world axis or local axis rotations by default.
	* Hide selected references, culling them from the scene so you can work behind them.
	* Restore hidden references, restoring to view any references hidden with the hide selection option.
	* Set or clear the QuickStart data from the current render window perspective.
	* Set the testing environment to use the selected reference position/rotation or that of the camera.
	* Hide landscape.
* The field of view (FOV) and antialiasing (multisamples) can be overwritten in the config file.
* The update rate of the render window can be changed from the default of 25 FPS. By default it now runs at (up to) 60 FPS.
* Fixed an issue with bound calculations. Any bound-related functions, such as focusing the camera (C or T keys) on a light will never send the camera into the stratosphere.
* New widgets exist when moving or rotating a reference. Axis lines are shown when snapping. Holding control while moving or rotating a reference will force on grid/angle snapping, and show markers to show where the reference will snap to. Using the mouse wheel while doing this will adjust the grid/angle snap values.
* Using F2 with a reference selected will bring up the window to edit that reference. Holding shift will bring up the window to edit the base object instead.
* Ctrl+H now opens the Search & Replace window.
* Ctrl+F2 with a door selected will focus the associated door marker. The same can be done on a door marker, to select the door that leads to it.

The following changes to the rendering window apply when landscape editing:

* The `F` and `S` keys can now be used to enter flatten and smooth vertices modes.
* The `[` and `]` keys can be used to decrement/increment the terrain editing radius.
* The terrain editing disc is no longer hidden when hiding objects (Shift+C).
* The terrain editing disc is colored based on the current editing mode. These colors are configurable.
* You can use control+right mouse button to sample the texture at the cursor's position. This will select the matching texture under the cursor automatically in the landscape editing menu.

### Search & Replace Window

* Optimized performance of initial display by 80% (e.g. 1.9 seconds instead of 9.7 seconds).

### Text Search Window

* Elements may now be double clicked to open the relevant edit window. The right context will be shown, if applicable. For example, a faction will be automatically selected, or a particular INFO record will be displayed.
* Full searches work correctly on long text, such as script or book text.
* Searches can be performed with regex, or made case-sensitive through the settings window.
* The search input box responds to basic keyboard shortcuts.

### Use Report

* Rows in the first list can now be double-clicked. Doing so will bring up a window to edit the related object.
