tes3gameSetting
====================================================================================================

A GMST game object.

Properties
----------------------------------------------------------------------------------------------------

`blocked`_ (`boolean`_)
    The blocked state of the object.

`defaultValue`_ (`number`_, `string`_)
    Read-only. The default value of the GMST, if no master defines the value.

`deleted`_ (`boolean`_)
    Read-only. The deleted state of the object.

`disabled`_ (`boolean`_)
    Read-only. The disabled state of the object.

`id`_ (`string`_)
    Read-only. The unique identifier for the object.

`index`_ (`number`_)
    Read-only. The array index for the GMST.

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

`type`_ (`string`_)
    Read-only. The type of the variable, either 'i', 'f', or 's'.

`value`_ (`number`_, `string`_)
    The value of the GMST.

.. toctree::
    :hidden:

    tes3gameSetting/blocked
    tes3gameSetting/defaultValue
    tes3gameSetting/deleted
    tes3gameSetting/disabled
    tes3gameSetting/id
    tes3gameSetting/index
    tes3gameSetting/modified
    tes3gameSetting/objectFlags
    tes3gameSetting/objectType
    tes3gameSetting/persistent
    tes3gameSetting/sourceMod
    tes3gameSetting/sourceless
    tes3gameSetting/supportsLuaData
    tes3gameSetting/type
    tes3gameSetting/value

.. _`blocked`: tes3gameSetting/blocked.html
.. _`defaultValue`: tes3gameSetting/defaultValue.html
.. _`deleted`: tes3gameSetting/deleted.html
.. _`disabled`: tes3gameSetting/disabled.html
.. _`id`: tes3gameSetting/id.html
.. _`index`: tes3gameSetting/index.html
.. _`modified`: tes3gameSetting/modified.html
.. _`objectFlags`: tes3gameSetting/objectFlags.html
.. _`objectType`: tes3gameSetting/objectType.html
.. _`persistent`: tes3gameSetting/persistent.html
.. _`sourceMod`: tes3gameSetting/sourceMod.html
.. _`sourceless`: tes3gameSetting/sourceless.html
.. _`supportsLuaData`: tes3gameSetting/supportsLuaData.html
.. _`type`: tes3gameSetting/type.html
.. _`value`: tes3gameSetting/value.html

Methods
----------------------------------------------------------------------------------------------------

`__tojson`_ (`string`_)
    Serializes the object to json.

.. toctree::
    :hidden:

    tes3gameSetting/__tojson

.. _`__tojson`: tes3gameSetting/__tojson.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
