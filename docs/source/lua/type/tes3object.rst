tes3object
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

    tes3object/deleted
    tes3object/disabled
    tes3object/id
    tes3object/modified
    tes3object/nextInCollection
    tes3object/objectFlags
    tes3object/objectType
    tes3object/owningCollection
    tes3object/previousInCollection
    tes3object/scale
    tes3object/sceneNode
    tes3object/sceneReference
    tes3object/sourceMod
    tes3object/supportsLuaData

.. _`deleted`: tes3object/deleted.html
.. _`disabled`: tes3object/disabled.html
.. _`id`: tes3object/id.html
.. _`modified`: tes3object/modified.html
.. _`nextInCollection`: tes3object/nextInCollection.html
.. _`objectFlags`: tes3object/objectFlags.html
.. _`objectType`: tes3object/objectType.html
.. _`owningCollection`: tes3object/owningCollection.html
.. _`previousInCollection`: tes3object/previousInCollection.html
.. _`scale`: tes3object/scale.html
.. _`sceneNode`: tes3object/sceneNode.html
.. _`sceneReference`: tes3object/sceneReference.html
.. _`sourceMod`: tes3object/sourceMod.html
.. _`supportsLuaData`: tes3object/supportsLuaData.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
