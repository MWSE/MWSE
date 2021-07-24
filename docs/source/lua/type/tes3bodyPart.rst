tes3bodyPart
====================================================================================================

A body part game object.

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

`female`_ (`boolean`_)
    A flag that marks this body part as used for female actors.

`id`_ (`string`_)
    Read-only. The unique identifier for the object.

`mesh`_ (`string`_)
    The path to the object's mesh.

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

`part`_ (`number`_)
    No description available.

`partType`_ (`number`_)
    No description available.

`persistent`_ (`boolean`_)
    The persistent flag of the object.

`playable`_ (`boolean`_)
    A flag that marks this body part as selectable during character generation.

`previousInCollection`_ (`tes3object`_)
    The previous object in parent collection's list.

`raceName`_ (`string`_)
    Read-only. The name of the race associated with this body part.

`scale`_ (`number`_)
    The object's scale.

`sceneNode`_ (`niNode`_)
    The scene graph node for this object.

`sceneNode`_ (`niNode`_)
    The NiNode-derived object for the object's loaded mesh.

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

`vampiric`_ (`boolean`_)
    A flag that marks this body part as used for vampires.

.. toctree::
    :hidden:

    tes3bodyPart/blocked
    tes3bodyPart/boundingBox
    tes3bodyPart/deleted
    tes3bodyPart/disabled
    tes3bodyPart/female
    tes3bodyPart/id
    tes3bodyPart/mesh
    tes3bodyPart/modified
    tes3bodyPart/nextInCollection
    tes3bodyPart/objectFlags
    tes3bodyPart/objectType
    tes3bodyPart/owningCollection
    tes3bodyPart/part
    tes3bodyPart/partType
    tes3bodyPart/persistent
    tes3bodyPart/playable
    tes3bodyPart/previousInCollection
    tes3bodyPart/raceName
    tes3bodyPart/scale
    tes3bodyPart/sceneNode
    tes3bodyPart/sceneNode
    tes3bodyPart/sceneReference
    tes3bodyPart/sourceMod
    tes3bodyPart/sourceless
    tes3bodyPart/stolenList
    tes3bodyPart/supportsLuaData
    tes3bodyPart/vampiric

.. _`blocked`: tes3bodyPart/blocked.html
.. _`boundingBox`: tes3bodyPart/boundingBox.html
.. _`deleted`: tes3bodyPart/deleted.html
.. _`disabled`: tes3bodyPart/disabled.html
.. _`female`: tes3bodyPart/female.html
.. _`id`: tes3bodyPart/id.html
.. _`mesh`: tes3bodyPart/mesh.html
.. _`modified`: tes3bodyPart/modified.html
.. _`nextInCollection`: tes3bodyPart/nextInCollection.html
.. _`objectFlags`: tes3bodyPart/objectFlags.html
.. _`objectType`: tes3bodyPart/objectType.html
.. _`owningCollection`: tes3bodyPart/owningCollection.html
.. _`part`: tes3bodyPart/part.html
.. _`partType`: tes3bodyPart/partType.html
.. _`persistent`: tes3bodyPart/persistent.html
.. _`playable`: tes3bodyPart/playable.html
.. _`previousInCollection`: tes3bodyPart/previousInCollection.html
.. _`raceName`: tes3bodyPart/raceName.html
.. _`scale`: tes3bodyPart/scale.html
.. _`sceneNode`: tes3bodyPart/sceneNode.html
.. _`sceneNode`: tes3bodyPart/sceneNode.html
.. _`sceneReference`: tes3bodyPart/sceneReference.html
.. _`sourceMod`: tes3bodyPart/sourceMod.html
.. _`sourceless`: tes3bodyPart/sourceless.html
.. _`stolenList`: tes3bodyPart/stolenList.html
.. _`supportsLuaData`: tes3bodyPart/supportsLuaData.html
.. _`vampiric`: tes3bodyPart/vampiric.html

Methods
----------------------------------------------------------------------------------------------------

`__tojson`_ (`string`_)
    Serializes the object to json.

.. toctree::
    :hidden:

    tes3bodyPart/__tojson

.. _`__tojson`: tes3bodyPart/__tojson.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
