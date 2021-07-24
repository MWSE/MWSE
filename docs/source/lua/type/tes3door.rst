tes3door
====================================================================================================

A door game object.

Properties
----------------------------------------------------------------------------------------------------

`blocked`_ (`boolean`_)
    The blocked state of the object.

`boundingBox`_ (`tes3boundingBox`_)
    The bounding box for the object.

`closeSound`_ (`tes3sound`_)
    The sound to be played when the door closes.

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

`openSound`_ (`tes3sound`_)
    The sound to be played when the door opens.

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
    Read-only. The script that runs on the object.

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

    tes3door/blocked
    tes3door/boundingBox
    tes3door/closeSound
    tes3door/deleted
    tes3door/disabled
    tes3door/id
    tes3door/mesh
    tes3door/modified
    tes3door/name
    tes3door/nextInCollection
    tes3door/objectFlags
    tes3door/objectType
    tes3door/openSound
    tes3door/owningCollection
    tes3door/persistent
    tes3door/previousInCollection
    tes3door/scale
    tes3door/sceneNode
    tes3door/sceneReference
    tes3door/script
    tes3door/sourceMod
    tes3door/sourceless
    tes3door/stolenList
    tes3door/supportsLuaData

.. _`blocked`: tes3door/blocked.html
.. _`boundingBox`: tes3door/boundingBox.html
.. _`closeSound`: tes3door/closeSound.html
.. _`deleted`: tes3door/deleted.html
.. _`disabled`: tes3door/disabled.html
.. _`id`: tes3door/id.html
.. _`mesh`: tes3door/mesh.html
.. _`modified`: tes3door/modified.html
.. _`name`: tes3door/name.html
.. _`nextInCollection`: tes3door/nextInCollection.html
.. _`objectFlags`: tes3door/objectFlags.html
.. _`objectType`: tes3door/objectType.html
.. _`openSound`: tes3door/openSound.html
.. _`owningCollection`: tes3door/owningCollection.html
.. _`persistent`: tes3door/persistent.html
.. _`previousInCollection`: tes3door/previousInCollection.html
.. _`scale`: tes3door/scale.html
.. _`sceneNode`: tes3door/sceneNode.html
.. _`sceneReference`: tes3door/sceneReference.html
.. _`script`: tes3door/script.html
.. _`sourceMod`: tes3door/sourceMod.html
.. _`sourceless`: tes3door/sourceless.html
.. _`stolenList`: tes3door/stolenList.html
.. _`supportsLuaData`: tes3door/supportsLuaData.html

Methods
----------------------------------------------------------------------------------------------------

`__tojson`_ (`string`_)
    Serializes the object to json.

.. toctree::
    :hidden:

    tes3door/__tojson

.. _`__tojson`: tes3door/__tojson.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
.. _`tes3sound`: ../../lua/type/tes3sound.html
