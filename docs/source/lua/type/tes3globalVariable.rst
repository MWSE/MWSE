tes3globalVariable
====================================================================================================

A global variable game object.

Properties
----------------------------------------------------------------------------------------------------

`deleted`_ (`boolean`_)
    The deleted state of the object.

`disabled`_ (`boolean`_)
    The disabled state of the object.

`id`_ (`string`_)
    The unique identifier for the object.

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

`value`_ (`number`_)
    The value of the variable. Unlike GMSTs, globals are always numbers.

.. toctree::
    :hidden:

    tes3globalVariable/deleted
    tes3globalVariable/disabled
    tes3globalVariable/id
    tes3globalVariable/modified
    tes3globalVariable/objectFlags
    tes3globalVariable/objectType
    tes3globalVariable/sourceMod
    tes3globalVariable/supportsLuaData
    tes3globalVariable/value

.. _`deleted`: tes3globalVariable/deleted.html
.. _`disabled`: tes3globalVariable/disabled.html
.. _`id`: tes3globalVariable/id.html
.. _`modified`: tes3globalVariable/modified.html
.. _`objectFlags`: tes3globalVariable/objectFlags.html
.. _`objectType`: tes3globalVariable/objectType.html
.. _`sourceMod`: tes3globalVariable/sourceMod.html
.. _`supportsLuaData`: tes3globalVariable/supportsLuaData.html
.. _`value`: tes3globalVariable/value.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
