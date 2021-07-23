tes3faction
====================================================================================================

A faction game object.

Properties
----------------------------------------------------------------------------------------------------

`attributes`_ (`table`_)
    Read-only. An array-style table holding the two attributes that govern advancement.

`deleted`_ (`boolean`_)
    The deleted state of the object.

`disabled`_ (`boolean`_)
    The disabled state of the object.

`id`_ (`string`_)
    The unique identifier for the object.

`modified`_ (`boolean`_)
    The modification state of the object since the last save.

`name`_ (`string`_)
    The faction's player-facing name.

`objectFlags`_ (`number`_)
    The raw flags of the object.

`objectType`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`playerExpelled`_ (`boolean`_)
    The player's expelled state in the faction.

`playerJoined`_ (`boolean`_)
    The player's join state for the faction.

`playerRank`_ (`number`_)
    The player's current rank in the faction.

`playerReputation`_ (`number`_)
    The player's current reputation in the faction.

`ranks`_ (`table`_)
    Read-only. An array-style table holding the ten related tes3factionRanks.

`reactions`_ (`tes3iterator`_)
    Read-only. A collection of tes3factionReactions.

`skills`_ (`table`_)
    Read-only. An array-style table holding the seven skills that govern advancement.

`sourceMod`_ (`string`_)
    The filename of the mod that owns this object.

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

.. toctree::
    :hidden:

    tes3faction/attributes
    tes3faction/deleted
    tes3faction/disabled
    tes3faction/id
    tes3faction/modified
    tes3faction/name
    tes3faction/objectFlags
    tes3faction/objectType
    tes3faction/playerExpelled
    tes3faction/playerJoined
    tes3faction/playerRank
    tes3faction/playerReputation
    tes3faction/ranks
    tes3faction/reactions
    tes3faction/skills
    tes3faction/sourceMod
    tes3faction/supportsLuaData

.. _`attributes`: tes3faction/attributes.html
.. _`deleted`: tes3faction/deleted.html
.. _`disabled`: tes3faction/disabled.html
.. _`id`: tes3faction/id.html
.. _`modified`: tes3faction/modified.html
.. _`name`: tes3faction/name.html
.. _`objectFlags`: tes3faction/objectFlags.html
.. _`objectType`: tes3faction/objectType.html
.. _`playerExpelled`: tes3faction/playerExpelled.html
.. _`playerJoined`: tes3faction/playerJoined.html
.. _`playerRank`: tes3faction/playerRank.html
.. _`playerReputation`: tes3faction/playerReputation.html
.. _`ranks`: tes3faction/ranks.html
.. _`reactions`: tes3faction/reactions.html
.. _`skills`: tes3faction/skills.html
.. _`sourceMod`: tes3faction/sourceMod.html
.. _`supportsLuaData`: tes3faction/supportsLuaData.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
