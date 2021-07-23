tes3gameSetting
====================================================================================================

A GMST game object.

Properties
----------------------------------------------------------------------------------------------------

`defaultValue`_ (`number`_, `string`_)
    Read-only. The default value of the GMST, if no master defines the value.

`deleted`_ (`boolean`_)
    The deleted state of the object.

`disabled`_ (`boolean`_)
    The disabled state of the object.

`id`_ (`string`_)
    The unique identifier for the object.

`index`_ (`number`_)
    Read-only. The array index for the GMST.

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

`type`_ (`string`_)
    Read-only. The type of the variable, either 'i', 'f', or 's'.

`value`_ (`number`_, `string`_)
    The value of the GMST.

.. toctree::
    :hidden:

    tes3gameSetting/defaultValue
    tes3gameSetting/deleted
    tes3gameSetting/disabled
    tes3gameSetting/id
    tes3gameSetting/index
    tes3gameSetting/modified
    tes3gameSetting/objectFlags
    tes3gameSetting/objectType
    tes3gameSetting/sourceMod
    tes3gameSetting/supportsLuaData
    tes3gameSetting/type
    tes3gameSetting/value

.. _`defaultValue`: tes3gameSetting/defaultValue.html
.. _`deleted`: tes3gameSetting/deleted.html
.. _`disabled`: tes3gameSetting/disabled.html
.. _`id`: tes3gameSetting/id.html
.. _`index`: tes3gameSetting/index.html
.. _`modified`: tes3gameSetting/modified.html
.. _`objectFlags`: tes3gameSetting/objectFlags.html
.. _`objectType`: tes3gameSetting/objectType.html
.. _`sourceMod`: tes3gameSetting/sourceMod.html
.. _`supportsLuaData`: tes3gameSetting/supportsLuaData.html
.. _`type`: tes3gameSetting/type.html
.. _`value`: tes3gameSetting/value.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
