tes3baseObject
====================================================================================================

Almost anything that can be represented in the Construction Set is based on this structure.

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

.. toctree::
    :hidden:

    tes3baseObject/deleted
    tes3baseObject/disabled
    tes3baseObject/id
    tes3baseObject/modified
    tes3baseObject/objectFlags
    tes3baseObject/objectType
    tes3baseObject/sourceMod

.. _`deleted`: tes3baseObject/deleted.html
.. _`disabled`: tes3baseObject/disabled.html
.. _`id`: tes3baseObject/id.html
.. _`modified`: tes3baseObject/modified.html
.. _`objectFlags`: tes3baseObject/objectFlags.html
.. _`objectType`: tes3baseObject/objectType.html
.. _`sourceMod`: tes3baseObject/sourceMod.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`string`: ../../lua/type/string.html
.. _`number`: ../../lua/type/number.html
