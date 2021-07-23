tes3race
====================================================================================================

A core object representing a character race.

Properties
----------------------------------------------------------------------------------------------------

`abilities`_ (`tes3spellList`_)
    Read-only. A list of abilities that NPCs of that race have access to. It is a tes3spellList, which is a list wrapper with helper functions. The actual list is accessed with .iterator. e.g. for _, ability in pairs(race.abilities.iterator) do print(ability.name) end

`baseAttributes`_ (`table`_)
    Read-only. Array-style table access to base 8 attributes for the race. Each element in the array is a tes3raceBaseAttribute.

`deleted`_ (`boolean`_)
    The deleted state of the object.

`description`_ (`string`_)
    Read-only. Loads from disk and returns the description of the race.

`disabled`_ (`boolean`_)
    The disabled state of the object.

`femaleBody`_ (`tes3raceBodyParts`_)
    Read-only. Access to all the body parts that will be used for female actors of this race.

`flags`_ (`number`_)
    Raw bit-based flags.

`height`_ (`tes3raceHeightWeight`_)
    Read-only. Access to the the height pair for males/females of the race.

`id`_ (`string`_)
    The unique identifier for the object.

`isBeast`_ (`boolean`_)
    Access to the beast race flag.

`isPlayable`_ (`boolean`_)
    Access to the playable race flag.

`maleBody`_ (`tes3raceBodyParts`_)
    Read-only. Access to all the body parts that will be used for male actors of this race.

`modified`_ (`boolean`_)
    The modification state of the object since the last save.

`name`_ (`string`_)
    Read-only. The player-facing name for the object.

`objectFlags`_ (`number`_)
    The raw flags of the object.

`objectType`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`skillBonuses`_ (`table`_)
    Read-only. Array-style table access for 7 skill bonuses for the race. Each element in the array is a tes3raceSkillBonus.

`sourceMod`_ (`string`_)
    The filename of the mod that owns this object.

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`weight`_ (`tes3raceHeightWeight`_)
    Read-only. Access to the the height pair for males/females of the race.

.. toctree::
    :hidden:

    tes3race/abilities
    tes3race/baseAttributes
    tes3race/deleted
    tes3race/description
    tes3race/disabled
    tes3race/femaleBody
    tes3race/flags
    tes3race/height
    tes3race/id
    tes3race/isBeast
    tes3race/isPlayable
    tes3race/maleBody
    tes3race/modified
    tes3race/name
    tes3race/objectFlags
    tes3race/objectType
    tes3race/skillBonuses
    tes3race/sourceMod
    tes3race/supportsLuaData
    tes3race/weight

.. _`abilities`: tes3race/abilities.html
.. _`baseAttributes`: tes3race/baseAttributes.html
.. _`deleted`: tes3race/deleted.html
.. _`description`: tes3race/description.html
.. _`disabled`: tes3race/disabled.html
.. _`femaleBody`: tes3race/femaleBody.html
.. _`flags`: tes3race/flags.html
.. _`height`: tes3race/height.html
.. _`id`: tes3race/id.html
.. _`isBeast`: tes3race/isBeast.html
.. _`isPlayable`: tes3race/isPlayable.html
.. _`maleBody`: tes3race/maleBody.html
.. _`modified`: tes3race/modified.html
.. _`name`: tes3race/name.html
.. _`objectFlags`: tes3race/objectFlags.html
.. _`objectType`: tes3race/objectType.html
.. _`skillBonuses`: tes3race/skillBonuses.html
.. _`sourceMod`: tes3race/sourceMod.html
.. _`supportsLuaData`: tes3race/supportsLuaData.html
.. _`weight`: tes3race/weight.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3raceBodyParts`: ../../lua/type/tes3raceBodyParts.html
.. _`tes3raceHeightWeight`: ../../lua/type/tes3raceHeightWeight.html
.. _`tes3spellList`: ../../lua/type/tes3spellList.html
