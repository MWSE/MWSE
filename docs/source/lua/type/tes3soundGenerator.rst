tes3soundGenerator
====================================================================================================

A sound generator object created in the TES3 CS

Properties
----------------------------------------------------------------------------------------------------

`deleted`_ (`boolean`_)
    The deleted state of the object.

`disabled`_ (`boolean`_)
    The disabled state of the object.

`id`_ (`string`_)
    The unique identifier for the object.

`id`_ (`string`_)
    Read-only. The unique identifier for the object.

`modified`_ (`boolean`_)
    The modification state of the object since the last save.

`objectFlags`_ (`number`_)
    The raw flags of the object.

`objectType`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`sound`_ (`tes3sound`_)
    Read-only. Sound object tied to this sound generator.

`sourceMod`_ (`string`_)
    The filename of the mod that owns this object.

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`type`_ (`number`_)
    Read-only. Maps to tes3.soundGenType constants.

.. toctree::
    :hidden:

    tes3soundGenerator/deleted
    tes3soundGenerator/disabled
    tes3soundGenerator/id
    tes3soundGenerator/id
    tes3soundGenerator/modified
    tes3soundGenerator/objectFlags
    tes3soundGenerator/objectType
    tes3soundGenerator/sound
    tes3soundGenerator/sourceMod
    tes3soundGenerator/supportsLuaData
    tes3soundGenerator/type

.. _`deleted`: tes3soundGenerator/deleted.html
.. _`disabled`: tes3soundGenerator/disabled.html
.. _`id`: tes3soundGenerator/id.html
.. _`id`: tes3soundGenerator/id.html
.. _`modified`: tes3soundGenerator/modified.html
.. _`objectFlags`: tes3soundGenerator/objectFlags.html
.. _`objectType`: tes3soundGenerator/objectType.html
.. _`sound`: tes3soundGenerator/sound.html
.. _`sourceMod`: tes3soundGenerator/sourceMod.html
.. _`supportsLuaData`: tes3soundGenerator/supportsLuaData.html
.. _`type`: tes3soundGenerator/type.html

Methods
----------------------------------------------------------------------------------------------------

`__tojson`_ (`string`_)
    Serializes the object to json.

.. toctree::
    :hidden:

    tes3soundGenerator/__tojson

.. _`__tojson`: tes3soundGenerator/__tojson.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3sound`: ../../lua/type/tes3sound.html
