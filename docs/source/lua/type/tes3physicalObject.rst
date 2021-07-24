tes3physicalObject
====================================================================================================

Almost anything that can be represented in the Construction Set is based on this structure.

Properties
----------------------------------------------------------------------------------------------------

`blocked`_ (`boolean`_)
    The blocked state of the object.

`boundingBox`_ (`tes3boundingBox`_)
    The bounding box for the object.

`deleted`_ (`boolean`_)
    Read-only. The deleted state of the object.

`disabled`_ (`boolean`_)
    Read-only. The disabled state of the object.

`id`_ (`string`_)
    Read-only. The unique identifier for the object.

`modified`_ (`boolean`_)
    The modification state of the object since the last save.

`nextInCollection`_ (`tes3object`_)
    The next object in parent collection's list.

`objectFlags`_ (`number`_)
    Read-only. The raw flags of the object.

`objectType`_ (`number`_)
    Read-only. The type of object. Maps to values in tes3.objectType.

`owningCollection`_ (`tes3referenceList`_)
    The collection responsible for holding this object.

`persistent`_ (`boolean`_)
    The persistent flag of the object.

`previousInCollection`_ (`tes3object`_)
    The previous object in parent collection's list.

`scale`_ (`number`_)
    The object's scale.

`sceneNode`_ (`niNode`_)
    The scene graph node for this object.

`sceneReference`_ (`niNode`_)
    The scene graph reference node for this object.

`sourceMod`_ (`string`_)
    Read-only. The filename of the mod that owns this object.

`sourceless`_ (`boolean`_)
    The soruceless flag of the object.

`stolenList`_ (`tes3iterator`_)
    A list of actors that the object has been stolen from.

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

.. toctree::
    :hidden:

    tes3physicalObject/blocked
    tes3physicalObject/boundingBox
    tes3physicalObject/deleted
    tes3physicalObject/disabled
    tes3physicalObject/id
    tes3physicalObject/modified
    tes3physicalObject/nextInCollection
    tes3physicalObject/objectFlags
    tes3physicalObject/objectType
    tes3physicalObject/owningCollection
    tes3physicalObject/persistent
    tes3physicalObject/previousInCollection
    tes3physicalObject/scale
    tes3physicalObject/sceneNode
    tes3physicalObject/sceneReference
    tes3physicalObject/sourceMod
    tes3physicalObject/sourceless
    tes3physicalObject/stolenList
    tes3physicalObject/supportsLuaData

.. _`blocked`: tes3physicalObject/blocked.html
.. _`boundingBox`: tes3physicalObject/boundingBox.html
.. _`deleted`: tes3physicalObject/deleted.html
.. _`disabled`: tes3physicalObject/disabled.html
.. _`id`: tes3physicalObject/id.html
.. _`modified`: tes3physicalObject/modified.html
.. _`nextInCollection`: tes3physicalObject/nextInCollection.html
.. _`objectFlags`: tes3physicalObject/objectFlags.html
.. _`objectType`: tes3physicalObject/objectType.html
.. _`owningCollection`: tes3physicalObject/owningCollection.html
.. _`persistent`: tes3physicalObject/persistent.html
.. _`previousInCollection`: tes3physicalObject/previousInCollection.html
.. _`scale`: tes3physicalObject/scale.html
.. _`sceneNode`: tes3physicalObject/sceneNode.html
.. _`sceneReference`: tes3physicalObject/sceneReference.html
.. _`sourceMod`: tes3physicalObject/sourceMod.html
.. _`sourceless`: tes3physicalObject/sourceless.html
.. _`stolenList`: tes3physicalObject/stolenList.html
.. _`supportsLuaData`: tes3physicalObject/supportsLuaData.html

Methods
----------------------------------------------------------------------------------------------------

`__tojson`_ (`string`_)
    Serializes the object to json.

.. toctree::
    :hidden:

    tes3physicalObject/__tojson

.. _`__tojson`: tes3physicalObject/__tojson.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
