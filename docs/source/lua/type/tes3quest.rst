tes3quest
====================================================================================================

A representation of a quest, with associated dialogue and info.

Properties
----------------------------------------------------------------------------------------------------

`deleted`_ (`boolean`_)
    The deleted state of the object.

`dialogue`_ (`tes3iterator`_)
    Read-only. A collection of dialogues associated with the quest.

`disabled`_ (`boolean`_)
    The disabled state of the object.

`id`_ (`string`_)
    The unique identifier for the object.

`info`_ (`tes3iterator`_)
    Read-only. A collection of dialogue info associated with the quest.

`modified`_ (`boolean`_)
    The modification state of the object since the last save.

`objectFlags`_ (`number`_)
    The raw flags of the object.

`objectType`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`sourceMod`_ (`string`_)
    The filename of the mod that owns this object.

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

.. toctree::
    :hidden:

    tes3quest/deleted
    tes3quest/dialogue
    tes3quest/disabled
    tes3quest/id
    tes3quest/info
    tes3quest/modified
    tes3quest/objectFlags
    tes3quest/objectType
    tes3quest/sourceMod
    tes3quest/supportsLuaData

.. _`deleted`: tes3quest/deleted.html
.. _`dialogue`: tes3quest/dialogue.html
.. _`disabled`: tes3quest/disabled.html
.. _`id`: tes3quest/id.html
.. _`info`: tes3quest/info.html
.. _`modified`: tes3quest/modified.html
.. _`objectFlags`: tes3quest/objectFlags.html
.. _`objectType`: tes3quest/objectType.html
.. _`sourceMod`: tes3quest/sourceMod.html
.. _`supportsLuaData`: tes3quest/supportsLuaData.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
