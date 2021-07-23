tes3globalVariable
====================================================================================================

A global variable game object.

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

`sourceMod`_ (`string`_)
    Read-only. The filename of the mod that owns this object.

`sourceless`_ (`boolean`_)
    The soruceless flag of the object.

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`value`_ (`number`_)
    The value of the variable. Unlike GMSTs, globals are always numbers.

.. toctree::
    :hidden:

    tes3globalVariable/blocked
    tes3globalVariable/deleted
    tes3globalVariable/disabled
    tes3globalVariable/id
    tes3globalVariable/modified
    tes3globalVariable/objectFlags
    tes3globalVariable/objectType
    tes3globalVariable/persistent
    tes3globalVariable/sourceMod
    tes3globalVariable/sourceless
    tes3globalVariable/supportsLuaData
    tes3globalVariable/value

.. _`blocked`: tes3globalVariable/blocked.html
.. _`deleted`: tes3globalVariable/deleted.html
.. _`disabled`: tes3globalVariable/disabled.html
.. _`id`: tes3globalVariable/id.html
.. _`modified`: tes3globalVariable/modified.html
.. _`objectFlags`: tes3globalVariable/objectFlags.html
.. _`objectType`: tes3globalVariable/objectType.html
.. _`persistent`: tes3globalVariable/persistent.html
.. _`sourceMod`: tes3globalVariable/sourceMod.html
.. _`sourceless`: tes3globalVariable/sourceless.html
.. _`supportsLuaData`: tes3globalVariable/supportsLuaData.html
.. _`value`: tes3globalVariable/value.html

Methods
----------------------------------------------------------------------------------------------------

`__tojson`_ (`string`_)
    Serializes the object to json.

.. toctree::
    :hidden:

    tes3globalVariable/__tojson

.. _`__tojson`: tes3globalVariable/__tojson.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
