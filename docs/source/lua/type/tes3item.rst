tes3item
====================================================================================================

An item object.

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

`nextInCollection`_ (`tes3object`_)
    The next object in parent collection's list.

`objectFlags`_ (`number`_)
    The raw flags of the object.

`objectType`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`owningCollection`_ (`tes3referenceList`_)
    The collection responsible for holding this object.

`previousInCollection`_ (`tes3object`_)
    The previous object in parent collection's list.

`scale`_ (`number`_)
    The object's scale.

`sceneNode`_ (`niNode`_)
    The scene graph node for this object.

`sceneReference`_ (`niNode`_)
    The scene graph reference node for this object.

`sourceMod`_ (`string`_)
    The filename of the mod that owns this object.

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

.. toctree::
    :hidden:

    tes3item/deleted
    tes3item/disabled
    tes3item/id
    tes3item/modified
    tes3item/nextInCollection
    tes3item/objectFlags
    tes3item/objectType
    tes3item/owningCollection
    tes3item/previousInCollection
    tes3item/scale
    tes3item/sceneNode
    tes3item/sceneReference
    tes3item/sourceMod
    tes3item/supportsLuaData

.. _`deleted`: tes3item/deleted.html
.. _`disabled`: tes3item/disabled.html
.. _`id`: tes3item/id.html
.. _`modified`: tes3item/modified.html
.. _`nextInCollection`: tes3item/nextInCollection.html
.. _`objectFlags`: tes3item/objectFlags.html
.. _`objectType`: tes3item/objectType.html
.. _`owningCollection`: tes3item/owningCollection.html
.. _`previousInCollection`: tes3item/previousInCollection.html
.. _`scale`: tes3item/scale.html
.. _`sceneNode`: tes3item/sceneNode.html
.. _`sceneReference`: tes3item/sceneReference.html
.. _`sourceMod`: tes3item/sourceMod.html
.. _`supportsLuaData`: tes3item/supportsLuaData.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
