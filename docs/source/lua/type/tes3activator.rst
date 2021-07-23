tes3activator
====================================================================================================

An activator game object.

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

`mesh`_ (`string`_)
    The path to the object's mesh.

`modified`_ (`boolean`_)
    The modification state of the object since the last save.

`name`_ (`string`_)
    The player-facing name for the object.

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

`script`_ (`tes3script`_)
    The script that runs on the object.

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

    tes3activator/blocked
    tes3activator/boundingBox
    tes3activator/deleted
    tes3activator/disabled
    tes3activator/id
    tes3activator/mesh
    tes3activator/modified
    tes3activator/name
    tes3activator/nextInCollection
    tes3activator/objectFlags
    tes3activator/objectType
    tes3activator/owningCollection
    tes3activator/persistent
    tes3activator/previousInCollection
    tes3activator/scale
    tes3activator/sceneNode
    tes3activator/sceneReference
    tes3activator/script
    tes3activator/sourceMod
    tes3activator/sourceless
    tes3activator/stolenList
    tes3activator/supportsLuaData

.. _`blocked`: tes3activator/blocked.html
.. _`boundingBox`: tes3activator/boundingBox.html
.. _`deleted`: tes3activator/deleted.html
.. _`disabled`: tes3activator/disabled.html
.. _`id`: tes3activator/id.html
.. _`mesh`: tes3activator/mesh.html
.. _`modified`: tes3activator/modified.html
.. _`name`: tes3activator/name.html
.. _`nextInCollection`: tes3activator/nextInCollection.html
.. _`objectFlags`: tes3activator/objectFlags.html
.. _`objectType`: tes3activator/objectType.html
.. _`owningCollection`: tes3activator/owningCollection.html
.. _`persistent`: tes3activator/persistent.html
.. _`previousInCollection`: tes3activator/previousInCollection.html
.. _`scale`: tes3activator/scale.html
.. _`sceneNode`: tes3activator/sceneNode.html
.. _`sceneReference`: tes3activator/sceneReference.html
.. _`script`: tes3activator/script.html
.. _`sourceMod`: tes3activator/sourceMod.html
.. _`sourceless`: tes3activator/sourceless.html
.. _`stolenList`: tes3activator/stolenList.html
.. _`supportsLuaData`: tes3activator/supportsLuaData.html

Methods
----------------------------------------------------------------------------------------------------

`__tojson`_ (`string`_)
    Serializes the object to json.

.. toctree::
    :hidden:

    tes3activator/__tojson

.. _`__tojson`: tes3activator/__tojson.html

Functions
----------------------------------------------------------------------------------------------------

`create`_
    Creates a new activator object, which will be stored as part of the current saved game.

.. toctree::
    :hidden:

    tes3activator/create

.. _`create`: tes3activator/create.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
