
Changelog
===========================================================

This page documents the various changes to the Morrowind Script Extender from version 2.0 onwards. Older versions are documented on `GitHub <https://github.com/MWSE/MWSE/blob/v0.9/CHANGELOG.md>`_.

More details about each function can be found under the New Functions page.

In-Development
-----------------------------------------------------------

These changes are available when building from source, or from `downloading the latest in-development build <https://github.com/MWSE/MWSE/releases/download/build-automatic/mwse.zip>`_.

The biggest change in this version is the introduction of `new Lua scripting functionality <lua/guide/introduction.html>`_. Scripts can be `overridden <lua/guide/script-overrides.html>`_, or `independently called <mwscript/functions/lua/xLuaRunScript.html>`_ to perform actions previously impossible by mwscript. There's also a powerful `event-driven <lua/event.html>`_ way to write scripted mods.

Additionally, arbitrary data can now be persistently stored in the save, attached to references. These references do not necessarily need a script.

Added
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- Crash information is now collected. When the game crashes, a file called *MWSE_MiniDump.dmp* will be created in the Morrowind install folder. Zipping up this file as well as the *MWSE.dll* and *MWSE.pdb* files and sending them to a MWSE maintainer will allow them to diagnose the cause of the crash and help prevent them in future versions.

- Event-driven lua scripting. See the `introduction <lua/guide/introduction.html>`_ for more information.

- In-development version updater. *MWSE-Update.exe* is now included in the installation. Running this program will automatically download and install the latest in-development version of MWSE.

- `Script overrides <lua/guide/script-overrides.html>`_, allowing Lua to override the behavior of a script. This enables modders to use vanilla mwscript for OpenMW compatibility, but add extra features if MWSE is installed.

- ``xActivate``: Allows the forced activation of one reference to another. This allows you to make the player or NPCs pick up objects, go through doors, and otherwise perform a normal activation. Unlike the vanilla ``Activate`` function, this one wil always work and accepts variable input.

- ``xLuaRunScript``: Loads and executes a lua file inside of *Data Files/MWSE/mods*. That script has the power to interact with the MWSE stack, and return any data it wants. This effectively lets custom Lua-based functions to be written so that mwscripts are not limited.

- ``xMyRef``: Allows an object to get a reference to itself. Moved from ``xGetRef`` to its own function for better compatibility.

Changed
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- Refactored TES3 type information under the hood to make development smoother.
- LuaJIT 2.0.5 is now a dependency of the project.

Fixed
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- Fixed an issue where strings would be redundantly stored in memory.
- ``xEquipmentList``: Now returns the ammunition count, instead of always returning 1 for the count.
- ``xFileReadString``/``xFileReadText``: Added support for non-CRLF line endings. Some Wrye Mash forks use these, which caused issues.
- ``xGetRef``: Reverted feature to get the current reference by calling ``xGetRef`` with a value of zero.
- ``xNextRef``: Prevented the game from crashing if a script erroneously used this function. This is a common mistake for modders to make, especially with how slow pre-2.0 MWSE was. Now this won't cause crashes between save/reload. This is still not encouraged, but at least won't have the potential to completely break a save with script errors.
- ``xSpellList``: No longer crashes when called on an actor that has no spells.

v2.0.0
-----------------------------------------------------------

This release marks a complete under the hood rewrite of how MWSE functions. Version 2.0 is designed to be 100% compatible with previous version of MWSE.

Added
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- ``xContentListFiltered``: Behaves like ``xContentList``, with an additional filter parameter passed. The filter allows filtering by object type and/or enchantment, and only objects matching that filter will be returned.

- ``xEquipmentList``: Behaves similar to ``xContentList``/``xContentListFiltered``, returning inventory information. This function allows looping over equipped items instead of all items, and also returns some subtype information. Usage is ``setx id count type subtype value weight name enchantId nextNode to xEquipmentList node typeFilter subTypeFilter``. The ``typeFilter`` parameter matches the ``type`` returned, and will restrict results to that item type (e.g. only clothing). The ``subTypeFilter`` allows specifying a weapon type or armor/clothing slot. The subtype index is one higher for both return values and parameters are one higher than in their normal objects (e.g. pants are index ``1`` instead of ``0``). If ``typeFilter`` or ``subTypeFilter`` are 0, no filtering is performed on those values.

- ``xGetAlchemyInfo``: Allows the fetching the effect count (and flags) for an alchemy object.

- ``xGetIngredientEffect``: Returns the effect id and skill or attribute id associated with the effect. E.g. ``setx effectId skillOrAttributeId to xGetIngredientEffect "ingred_alit_hide_01" 1`` gets the first effect the ingredient has.

- ``xGetItemCount``: This function behaves like the vanilla ``GetItemCount``, but accepts a variable string input.

- ``xGetKeyBind``: Obtains the key codes used for input (e.g. what the current activation key is). Also supplies information such as if an input is bound to a keyboard, joystick, or mouse.

- ``xGetMCPFeatureState``: The `Morrowind Code Patch <https://www.nexusmods.com/morrowind/mods/19510/?>`_ has become more important with its fixes to the script parser, and other scripting extensions. If the user has the MCP installed and hasn't deleted the mcpatch folder, this function will let the script know if a MCP feature is enabled or disabled. The ID passed to this function can be found in the describe.json file in the mcpatch folder.

- ``xGetModel``: This function gets the model of either a reference or a object. It will work on anything but NPC or creature references, which return ``0``.

- ``xGetStackSize``: This function returns the size of a stack. If it can't find the stack information, it returns ``0``.

- ``xSetIngredientEffect``: Sets an ingredient's effect at a given index. E.g. ``xSetIngredientEffect "ingred_alit_hide_01" 1 83 4`` sets the first effect of alit hide to be fortify speed.

- ``xStartScript``, ``xStopScript`` and ``xScriptRunning``: These functions are wrappers around their non-MWSE counterparts, and accept a variable input. Additionally, calling ``xStopScript 0`` will effectively call ``xStopScript`` on the current script.

- ``xStringCapture``: Performs a regex match using a given pattern on a given string. Usage is ``setx match1 match2 match3 to xStringCapture string pattern numResults``. The ``numResults`` value must match the number of return values desired. If there aren't enough matches to fill each return value, or if there isn't a capture, the return (extra) return values will be ``0``.

Changed
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- Performance improvements across the board. The underlying mechanism used to extend Morrowind's scripting system has been entirely rewritten to improve performance. Performance gains as high as 90 FPS on heavy MWSE-scripted modlists have been objected. Mods such as `MWSE Containers <https://www.nexusmods.com/morrowind/mods/44387/?>`_ and `MWSE Alchemy Filters <https://www.nexusmods.com/morrowind/mods/44808?>`_ should no longer cause long hiccups and mods like `The Bare Necessities <https://www.nexusmods.com/morrowind/mods/43365/?>`_ should not cause such severe performance hits. This opens new scripting opportunities that previously were not viable due to the old version's performance issues.

- String storage has changed under the hood. It's just as reliable as before, but is more recognizable. Strings are stored by ID rather than as string pointers, starting at an index of 40,000. An invalid/empty response is still treated as zero. Scripters do not need to change how they handle strings in any way. This has the added side-benefit that string IDs can more reliably be stored in globals.

- Version information has adapted to something more traditional. MWSE is mature software that has been out for many years. It isn't in alpha or beta anymore. As such, it is treating pre-2.0 MWSE as version 1, and making the leap to version 2 with this entire rewrite.

- ``xStringParse``: This function can now be used to determine MWSE's version number at runtime. This can be useful for ensuring that a feature is available. Correct usage is ``setx version versionCheck to xStringParse "MWSE_VERSION" versionNumberDesired``.

    - The first returned value is a ``long`` with a value starting at ``2000000`` for version 2.0.0. This continues to a pattern, where the hypothetical version 5.59.3 would return ``5059003``. On versions of MWSE prior to 2.0, ``xStringParse`` will return ``0``.

    - The second parameter can be used as a version check, and the second returned value will be ``1`` if the current version meets that requirement. For example, ``setx version versionCheck to xStringParse "MWSE_VERSION" 2001000`` will set ``versionCheck`` to ``1`` if the current MWSE version is at least 2.1.0. Otherwise ``versionCheck`` will be ``0``.

- ``xGetValue`` and ``xSetValue`` now work on gold without hardcoding values.

- ``xAddEffect``, ``xDeleteEffect``, ``xGetEffectInfo``, and ``xSetEffectInfo`` now support ``ALCH`` (potion) objects.

- ``xGetRef`` now returns a reference to its caller when given the argument ``0``.

Fixed
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- ``xAITravel``, ``xStartCombat``, ``xDistance``, ``xGetCombat``, ``xInventory``, ``xRefID``, and ``xRefType`` no longer cause a crash when called by an invalid reference.

- ``xGetRace`` no longer leaks memory. Arrays returned by this function are reused and refreshed with the newest information on each call.

- ``xPlace``'s returned reference can now be reliably used in the same frame that it was created.

- ``xSetBaseEffectInfo`` correctly functions. Previously it had a bug that prevented it from actually setting any values.

- ``xSetName`` is now safe to use, and no longer rewrites random portions of memory. This function used to cause various issues if the new name was longer than the old name on most object types. This could cause random value changes, and make the game prone to crashing.

- ``xSetQuality`` now works correctly on apparatus.

- ``xSetWeight`` now works correctly on lights.

- ``xStringLength`` now correctly returns 0 when called on an empty string.

Known Issues
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- ``xEquipmentList``: The count returned is always ``1``, though more than one ammo item could be equipped.

- ``xGetEncumbrance``: This function is unreliable and should not be used yet.
