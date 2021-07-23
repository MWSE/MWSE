tes3quest
====================================================================================================

A representation of a quest, with associated dialogue and info.

Properties
----------------------------------------------------------------------------------------------------

`blocked`_ (`boolean`_)
    The blocked state of the object.

`deleted`_ (`boolean`_)
    Read-only. The deleted state of the object.

`dialogue`_ (`tes3iterator`_)
    Read-only. A collection of dialogues associated with the quest.

`disabled`_ (`boolean`_)
    Read-only. The disabled state of the object.

`id`_ (`string`_)
    Read-only. The unique identifier for the object.

`info`_ (`tes3iterator`_)
    Read-only. A collection of dialogue info associated with the quest.

`modified`_ (`boolean`_)
    The modification state of the object since the last save.

`objectFlags`_ (`number`_)
    Read-only. The raw flags of the object.

`objectType`_ (`number`_)
    Read-only. The type of object. Maps to values in tes3.objectType.

`persistent`_ (`boolean`_)
    The persistent flag of the object.

`sourceMod`_ (`string`_)
    Read-only. The filename of the mod that owns this object.

`sourceless`_ (`boolean`_)
    The soruceless flag of the object.

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

.. toctree::
    :hidden:

    tes3quest/blocked
    tes3quest/deleted
    tes3quest/dialogue
    tes3quest/disabled
    tes3quest/id
    tes3quest/info
    tes3quest/modified
    tes3quest/objectFlags
    tes3quest/objectType
    tes3quest/persistent
    tes3quest/sourceMod
    tes3quest/sourceless
    tes3quest/supportsLuaData

.. _`blocked`: tes3quest/blocked.html
.. _`deleted`: tes3quest/deleted.html
.. _`dialogue`: tes3quest/dialogue.html
.. _`disabled`: tes3quest/disabled.html
.. _`id`: tes3quest/id.html
.. _`info`: tes3quest/info.html
.. _`modified`: tes3quest/modified.html
.. _`objectFlags`: tes3quest/objectFlags.html
.. _`objectType`: tes3quest/objectType.html
.. _`persistent`: tes3quest/persistent.html
.. _`sourceMod`: tes3quest/sourceMod.html
.. _`sourceless`: tes3quest/sourceless.html
.. _`supportsLuaData`: tes3quest/supportsLuaData.html

Methods
----------------------------------------------------------------------------------------------------

`__tojson`_ (`string`_)
    Serializes the object to json.

.. toctree::
    :hidden:

    tes3quest/__tojson

.. _`__tojson`: tes3quest/__tojson.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
