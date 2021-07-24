tes3baseObject
====================================================================================================

Almost anything that can be represented in the Construction Set is based on this structure.

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

.. toctree::
    :hidden:

    tes3baseObject/blocked
    tes3baseObject/deleted
    tes3baseObject/disabled
    tes3baseObject/id
    tes3baseObject/modified
    tes3baseObject/objectFlags
    tes3baseObject/objectType
    tes3baseObject/persistent
    tes3baseObject/sourceMod
    tes3baseObject/sourceless
    tes3baseObject/supportsLuaData

.. _`blocked`: tes3baseObject/blocked.html
.. _`deleted`: tes3baseObject/deleted.html
.. _`disabled`: tes3baseObject/disabled.html
.. _`id`: tes3baseObject/id.html
.. _`modified`: tes3baseObject/modified.html
.. _`objectFlags`: tes3baseObject/objectFlags.html
.. _`objectType`: tes3baseObject/objectType.html
.. _`persistent`: tes3baseObject/persistent.html
.. _`sourceMod`: tes3baseObject/sourceMod.html
.. _`sourceless`: tes3baseObject/sourceless.html
.. _`supportsLuaData`: tes3baseObject/supportsLuaData.html

Methods
----------------------------------------------------------------------------------------------------

`__tojson`_ (`string`_)
    Serializes the object to json.

.. toctree::
    :hidden:

    tes3baseObject/__tojson

.. _`__tojson`: tes3baseObject/__tojson.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
