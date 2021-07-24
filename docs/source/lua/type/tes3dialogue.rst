tes3dialogue
====================================================================================================

A parent-level dialogue, such as a topic, voice, greeting, persuasion response, or journal.

Properties
----------------------------------------------------------------------------------------------------

`blocked`_ (`boolean`_)
    The blocked state of the object.

`deleted`_ (`boolean`_)
    Read-only. The deleted state of the object.

`disabled`_ (`boolean`_)
    Read-only. The disabled state of the object.

`id`_ (`string`_)
    Read-only. The unique identifier for the object.

`info`_ (`tes3iterator`_)
    Read-only. A collection of individual entries in the dialogue.

`journalIndex`_ (`number`_)
    For journal dialogues, the currently active journal index.

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

`type`_ (`number`_)
    Read-only. The type of the dialogue.

.. toctree::
    :hidden:

    tes3dialogue/blocked
    tes3dialogue/deleted
    tes3dialogue/disabled
    tes3dialogue/id
    tes3dialogue/info
    tes3dialogue/journalIndex
    tes3dialogue/modified
    tes3dialogue/objectFlags
    tes3dialogue/objectType
    tes3dialogue/persistent
    tes3dialogue/sourceMod
    tes3dialogue/sourceless
    tes3dialogue/supportsLuaData
    tes3dialogue/type

.. _`blocked`: tes3dialogue/blocked.html
.. _`deleted`: tes3dialogue/deleted.html
.. _`disabled`: tes3dialogue/disabled.html
.. _`id`: tes3dialogue/id.html
.. _`info`: tes3dialogue/info.html
.. _`journalIndex`: tes3dialogue/journalIndex.html
.. _`modified`: tes3dialogue/modified.html
.. _`objectFlags`: tes3dialogue/objectFlags.html
.. _`objectType`: tes3dialogue/objectType.html
.. _`persistent`: tes3dialogue/persistent.html
.. _`sourceMod`: tes3dialogue/sourceMod.html
.. _`sourceless`: tes3dialogue/sourceless.html
.. _`supportsLuaData`: tes3dialogue/supportsLuaData.html
.. _`type`: tes3dialogue/type.html

Methods
----------------------------------------------------------------------------------------------------

`__tojson`_ (`string`_)
    Serializes the object to json.

`addToJournal`_ (`boolean`_)
    Adds the dialogue to the player's journal, if applicable, at a given index.

`getInfo`_ (`tes3dialogueInfo`_)
    Fetches the info that a given actor would produce for the dialogue.

.. toctree::
    :hidden:

    tes3dialogue/__tojson
    tes3dialogue/addToJournal
    tes3dialogue/getInfo

.. _`__tojson`: tes3dialogue/__tojson.html
.. _`addToJournal`: tes3dialogue/addToJournal.html
.. _`getInfo`: tes3dialogue/getInfo.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3dialogueInfo`: ../../lua/type/tes3dialogueInfo.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
