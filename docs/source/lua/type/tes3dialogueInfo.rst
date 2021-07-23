tes3dialogueInfo
====================================================================================================

A child for a given dialogue. Whereas a dialogue may be a conversation topic, a tes3dialogueInfo would be an individual response.

Properties
----------------------------------------------------------------------------------------------------

`actor`_ (`tes3actor`_)
    Read-only. The speaker's actor that the info is filtered for.

`cell`_ (`tes3cell`_)
    Read-only. The speaker's current cell that the info is filtered for.

`deleted`_ (`boolean`_)
    The deleted state of the object.

`disabled`_ (`boolean`_)
    The disabled state of the object.

`disposition`_ (`number`_)
    Read-only. The minimum disposition that the info is filtered for.

`firstHeardFrom`_ (`tes3actor`_)
    The actor that the player first heard the info from.

`id`_ (`string`_)
    The unique identifier for the object.

`id`_ (`string`_)
    Read-only. The unique long string ID for the info. This is not kept in memory, and must be loaded from files for each call.

`modified`_ (`boolean`_)
    The modification state of the object since the last save.

`npcClass`_ (`tes3class`_)
    Read-only. The speaker's class that the info is filtered for.

`npcFaction`_ (`tes3faction`_)
    Read-only. The speaker's faction that the info is filtered for.

`npcRace`_ (`tes3actor`_)
    Read-only. The speaker's race that the info is filtered for.

`npcRank`_ (`number`_)
    Read-only. The speaker's faction rank that the info is filtered for.

`npcSex`_ (`number`_)
    Read-only. The speaker's sex that the info is filtered for.

`objectFlags`_ (`number`_)
    The raw flags of the object.

`objectType`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`pcFaction`_ (`number`_)
    Read-only. The player's joined faction that the info is filtered for.

`pcRank`_ (`number`_)
    Read-only. The player's rank required rank in the speaker's faction.

`sourceMod`_ (`string`_)
    The filename of the mod that owns this object.

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`text`_ (`string`_)
    Read-only. String contents for the info. This is not kept in memory, and must be loaded from files for each call.

`type`_ (`number`_)
    Read-only. The type of the info.

.. toctree::
    :hidden:

    tes3dialogueInfo/actor
    tes3dialogueInfo/cell
    tes3dialogueInfo/deleted
    tes3dialogueInfo/disabled
    tes3dialogueInfo/disposition
    tes3dialogueInfo/firstHeardFrom
    tes3dialogueInfo/id
    tes3dialogueInfo/id
    tes3dialogueInfo/modified
    tes3dialogueInfo/npcClass
    tes3dialogueInfo/npcFaction
    tes3dialogueInfo/npcRace
    tes3dialogueInfo/npcRank
    tes3dialogueInfo/npcSex
    tes3dialogueInfo/objectFlags
    tes3dialogueInfo/objectType
    tes3dialogueInfo/pcFaction
    tes3dialogueInfo/pcRank
    tes3dialogueInfo/sourceMod
    tes3dialogueInfo/supportsLuaData
    tes3dialogueInfo/text
    tes3dialogueInfo/type

.. _`actor`: tes3dialogueInfo/actor.html
.. _`cell`: tes3dialogueInfo/cell.html
.. _`deleted`: tes3dialogueInfo/deleted.html
.. _`disabled`: tes3dialogueInfo/disabled.html
.. _`disposition`: tes3dialogueInfo/disposition.html
.. _`firstHeardFrom`: tes3dialogueInfo/firstHeardFrom.html
.. _`id`: tes3dialogueInfo/id.html
.. _`id`: tes3dialogueInfo/id.html
.. _`modified`: tes3dialogueInfo/modified.html
.. _`npcClass`: tes3dialogueInfo/npcClass.html
.. _`npcFaction`: tes3dialogueInfo/npcFaction.html
.. _`npcRace`: tes3dialogueInfo/npcRace.html
.. _`npcRank`: tes3dialogueInfo/npcRank.html
.. _`npcSex`: tes3dialogueInfo/npcSex.html
.. _`objectFlags`: tes3dialogueInfo/objectFlags.html
.. _`objectType`: tes3dialogueInfo/objectType.html
.. _`pcFaction`: tes3dialogueInfo/pcFaction.html
.. _`pcRank`: tes3dialogueInfo/pcRank.html
.. _`sourceMod`: tes3dialogueInfo/sourceMod.html
.. _`supportsLuaData`: tes3dialogueInfo/supportsLuaData.html
.. _`text`: tes3dialogueInfo/text.html
.. _`type`: tes3dialogueInfo/type.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3actor`: ../../lua/type/tes3actor.html
.. _`tes3cell`: ../../lua/type/tes3cell.html
.. _`tes3class`: ../../lua/type/tes3class.html
.. _`tes3faction`: ../../lua/type/tes3faction.html
