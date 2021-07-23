tes3physicalObject
====================================================================================================

Almost anything that can be represented in the Construction Set is based on this structure.

Properties
----------------------------------------------------------------------------------------------------

`boundingBox`_ (`tes3boundingBox`_)
    The bounding box for the object.

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

`stolenList`_ (`tes3iterator`_)
    A list of actors that the object has been stolen from.

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

.. toctree::
    :hidden:

    tes3physicalObject/boundingBox
    tes3physicalObject/deleted
    tes3physicalObject/disabled
    tes3physicalObject/id
    tes3physicalObject/modified
    tes3physicalObject/nextInCollection
    tes3physicalObject/objectFlags
    tes3physicalObject/objectType
    tes3physicalObject/owningCollection
    tes3physicalObject/previousInCollection
    tes3physicalObject/scale
    tes3physicalObject/sceneNode
    tes3physicalObject/sceneReference
    tes3physicalObject/sourceMod
    tes3physicalObject/stolenList
    tes3physicalObject/supportsLuaData

.. _`boundingBox`: tes3physicalObject/boundingBox.html
.. _`deleted`: tes3physicalObject/deleted.html
.. _`disabled`: tes3physicalObject/disabled.html
.. _`id`: tes3physicalObject/id.html
.. _`modified`: tes3physicalObject/modified.html
.. _`nextInCollection`: tes3physicalObject/nextInCollection.html
.. _`objectFlags`: tes3physicalObject/objectFlags.html
.. _`objectType`: tes3physicalObject/objectType.html
.. _`owningCollection`: tes3physicalObject/owningCollection.html
.. _`previousInCollection`: tes3physicalObject/previousInCollection.html
.. _`scale`: tes3physicalObject/scale.html
.. _`sceneNode`: tes3physicalObject/sceneNode.html
.. _`sceneReference`: tes3physicalObject/sceneReference.html
.. _`sourceMod`: tes3physicalObject/sourceMod.html
.. _`stolenList`: tes3physicalObject/stolenList.html
.. _`supportsLuaData`: tes3physicalObject/supportsLuaData.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
