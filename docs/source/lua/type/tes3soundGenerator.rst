tes3soundGenerator
====================================================================================================

A sound generator object created in the TES3 CS

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

`modified`_ (`boolean`_)
    The modification state of the object since the last save.

`objectFlags`_ (`number`_)
    Read-only. The raw flags of the object.

`objectType`_ (`number`_)
    Read-only. The type of object. Maps to values in tes3.objectType.

`persistent`_ (`boolean`_)
    The persistent flag of the object.

`sound`_ (`tes3sound`_)
    Read-only. Sound object tied to this sound generator.

`sourceMod`_ (`string`_)
    Read-only. The filename of the mod that owns this object.

`sourceless`_ (`boolean`_)
    The soruceless flag of the object.

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`type`_ (`number`_)
    Read-only. Maps to tes3.soundGenType constants.

.. toctree::
    :hidden:

    tes3soundGenerator/blocked
    tes3soundGenerator/deleted
    tes3soundGenerator/disabled
    tes3soundGenerator/id
    tes3soundGenerator/modified
    tes3soundGenerator/objectFlags
    tes3soundGenerator/objectType
    tes3soundGenerator/persistent
    tes3soundGenerator/sound
    tes3soundGenerator/sourceMod
    tes3soundGenerator/sourceless
    tes3soundGenerator/supportsLuaData
    tes3soundGenerator/type

.. _`blocked`: tes3soundGenerator/blocked.html
.. _`deleted`: tes3soundGenerator/deleted.html
.. _`disabled`: tes3soundGenerator/disabled.html
.. _`id`: tes3soundGenerator/id.html
.. _`modified`: tes3soundGenerator/modified.html
.. _`objectFlags`: tes3soundGenerator/objectFlags.html
.. _`objectType`: tes3soundGenerator/objectType.html
.. _`persistent`: tes3soundGenerator/persistent.html
.. _`sound`: tes3soundGenerator/sound.html
.. _`sourceMod`: tes3soundGenerator/sourceMod.html
.. _`sourceless`: tes3soundGenerator/sourceless.html
.. _`supportsLuaData`: tes3soundGenerator/supportsLuaData.html
.. _`type`: tes3soundGenerator/type.html

Methods
----------------------------------------------------------------------------------------------------

`__tojson`_ (`string`_)
    Serializes the object to json.

`__tojson`_ (`string`_)
    Serializes the object to json.

.. toctree::
    :hidden:

    tes3soundGenerator/__tojson
    tes3soundGenerator/__tojson

.. _`__tojson`: tes3soundGenerator/__tojson.html
.. _`__tojson`: tes3soundGenerator/__tojson.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3sound`: ../../lua/type/tes3sound.html
