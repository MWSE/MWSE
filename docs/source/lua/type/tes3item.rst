tes3item
====================================================================================================

An item object.

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

    tes3item/blocked
    tes3item/boundingBox
    tes3item/deleted
    tes3item/disabled
    tes3item/id
    tes3item/modified
    tes3item/nextInCollection
    tes3item/objectFlags
    tes3item/objectType
    tes3item/owningCollection
    tes3item/persistent
    tes3item/previousInCollection
    tes3item/scale
    tes3item/sceneNode
    tes3item/sceneReference
    tes3item/sourceMod
    tes3item/sourceless
    tes3item/stolenList
    tes3item/supportsLuaData

.. _`blocked`: tes3item/blocked.html
.. _`boundingBox`: tes3item/boundingBox.html
.. _`deleted`: tes3item/deleted.html
.. _`disabled`: tes3item/disabled.html
.. _`id`: tes3item/id.html
.. _`modified`: tes3item/modified.html
.. _`nextInCollection`: tes3item/nextInCollection.html
.. _`objectFlags`: tes3item/objectFlags.html
.. _`objectType`: tes3item/objectType.html
.. _`owningCollection`: tes3item/owningCollection.html
.. _`persistent`: tes3item/persistent.html
.. _`previousInCollection`: tes3item/previousInCollection.html
.. _`scale`: tes3item/scale.html
.. _`sceneNode`: tes3item/sceneNode.html
.. _`sceneReference`: tes3item/sceneReference.html
.. _`sourceMod`: tes3item/sourceMod.html
.. _`sourceless`: tes3item/sourceless.html
.. _`stolenList`: tes3item/stolenList.html
.. _`supportsLuaData`: tes3item/supportsLuaData.html

Methods
----------------------------------------------------------------------------------------------------

`__tojson`_ (`string`_)
    Serializes the object to json.

.. toctree::
    :hidden:

    tes3item/__tojson

.. _`__tojson`: tes3item/__tojson.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
