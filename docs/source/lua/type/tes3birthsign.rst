tes3birthsign
====================================================================================================

An object representing a birthsign.

Properties
----------------------------------------------------------------------------------------------------

`blocked`_ (`boolean`_)
    The blocked state of the object.

`deleted`_ (`boolean`_)
    Read-only. The deleted state of the object.

`description`_ (`string`_)
    Read-only. Loads from disk and returns the description of the birthsign.

`disabled`_ (`boolean`_)
    Read-only. The disabled state of the object.

`id`_ (`string`_)
    Read-only. The unique identifier for the object.

`modified`_ (`boolean`_)
    The modification state of the object since the last save.

`name`_ (`string`_)
    The player-facing name for the object.

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

`spells`_ (`tes3spellList`_)
    Read-only. A list of spells granted by the birthsign.

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`texturePath`_ (`string`_)
    Path to the related image representing the birthsign.

.. toctree::
    :hidden:

    tes3birthsign/blocked
    tes3birthsign/deleted
    tes3birthsign/description
    tes3birthsign/disabled
    tes3birthsign/id
    tes3birthsign/modified
    tes3birthsign/name
    tes3birthsign/objectFlags
    tes3birthsign/objectType
    tes3birthsign/persistent
    tes3birthsign/sourceMod
    tes3birthsign/sourceless
    tes3birthsign/spells
    tes3birthsign/supportsLuaData
    tes3birthsign/texturePath

.. _`blocked`: tes3birthsign/blocked.html
.. _`deleted`: tes3birthsign/deleted.html
.. _`description`: tes3birthsign/description.html
.. _`disabled`: tes3birthsign/disabled.html
.. _`id`: tes3birthsign/id.html
.. _`modified`: tes3birthsign/modified.html
.. _`name`: tes3birthsign/name.html
.. _`objectFlags`: tes3birthsign/objectFlags.html
.. _`objectType`: tes3birthsign/objectType.html
.. _`persistent`: tes3birthsign/persistent.html
.. _`sourceMod`: tes3birthsign/sourceMod.html
.. _`sourceless`: tes3birthsign/sourceless.html
.. _`spells`: tes3birthsign/spells.html
.. _`supportsLuaData`: tes3birthsign/supportsLuaData.html
.. _`texturePath`: tes3birthsign/texturePath.html

Methods
----------------------------------------------------------------------------------------------------

`__tojson`_ (`string`_)
    Serializes the object to json.

.. toctree::
    :hidden:

    tes3birthsign/__tojson

.. _`__tojson`: tes3birthsign/__tojson.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3spellList`: ../../lua/type/tes3spellList.html
