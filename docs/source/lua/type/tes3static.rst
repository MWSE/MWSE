tes3static
====================================================================================================

A static game object.

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

`mesh`_ (`string`_)
    The path to the object's mesh.

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

    tes3static/boundingBox
    tes3static/deleted
    tes3static/disabled
    tes3static/id
    tes3static/mesh
    tes3static/modified
    tes3static/nextInCollection
    tes3static/objectFlags
    tes3static/objectType
    tes3static/owningCollection
    tes3static/previousInCollection
    tes3static/scale
    tes3static/sceneNode
    tes3static/sceneReference
    tes3static/sourceMod
    tes3static/stolenList
    tes3static/supportsLuaData

.. _`boundingBox`: tes3static/boundingBox.html
.. _`deleted`: tes3static/deleted.html
.. _`disabled`: tes3static/disabled.html
.. _`id`: tes3static/id.html
.. _`mesh`: tes3static/mesh.html
.. _`modified`: tes3static/modified.html
.. _`nextInCollection`: tes3static/nextInCollection.html
.. _`objectFlags`: tes3static/objectFlags.html
.. _`objectType`: tes3static/objectType.html
.. _`owningCollection`: tes3static/owningCollection.html
.. _`previousInCollection`: tes3static/previousInCollection.html
.. _`scale`: tes3static/scale.html
.. _`sceneNode`: tes3static/sceneNode.html
.. _`sceneReference`: tes3static/sceneReference.html
.. _`sourceMod`: tes3static/sourceMod.html
.. _`stolenList`: tes3static/stolenList.html
.. _`supportsLuaData`: tes3static/supportsLuaData.html

Functions
----------------------------------------------------------------------------------------------------

`create`_
    Creates a new static object, which will be stored as part of the current saved game.

.. toctree::
    :hidden:

    tes3static/create

.. _`create`: tes3static/create.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
