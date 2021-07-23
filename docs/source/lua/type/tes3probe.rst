tes3probe
====================================================================================================

A core lockpick object.

Properties
----------------------------------------------------------------------------------------------------

`boundingBox`_ (`tes3boundingBox`_)
    The bounding box for the object.

`condition`_ (`number`_)
    The maximum condition/health of the object.

`deleted`_ (`boolean`_)
    The deleted state of the object.

`disabled`_ (`boolean`_)
    The disabled state of the object.

`icon`_ (`string`_)
    The path to the object's icon.

`id`_ (`string`_)
    The unique identifier for the object.

`mesh`_ (`string`_)
    The path to the object's mesh.

`modified`_ (`boolean`_)
    The modification state of the object since the last save.

`name`_ (`string`_)
    The player-facing name for the object.

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

`quality`_ (`number`_)
    The quality of the object, with how much of a bonus it has.

`scale`_ (`number`_)
    The object's scale.

`sceneNode`_ (`niNode`_)
    The scene graph node for this object.

`sceneReference`_ (`niNode`_)
    The scene graph reference node for this object.

`script`_ (`tes3script`_)
    Read-only. The script that runs on the object.

`sourceMod`_ (`string`_)
    The filename of the mod that owns this object.

`stolenList`_ (`tes3iterator`_)
    A list of actors that the object has been stolen from.

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`value`_ (`number`_)
    The value of the object.

`weight`_ (`number`_)
    The weight of the object.

.. toctree::
    :hidden:

    tes3probe/boundingBox
    tes3probe/condition
    tes3probe/deleted
    tes3probe/disabled
    tes3probe/icon
    tes3probe/id
    tes3probe/mesh
    tes3probe/modified
    tes3probe/name
    tes3probe/nextInCollection
    tes3probe/objectFlags
    tes3probe/objectType
    tes3probe/owningCollection
    tes3probe/previousInCollection
    tes3probe/quality
    tes3probe/scale
    tes3probe/sceneNode
    tes3probe/sceneReference
    tes3probe/script
    tes3probe/sourceMod
    tes3probe/stolenList
    tes3probe/supportsLuaData
    tes3probe/value
    tes3probe/weight

.. _`boundingBox`: tes3probe/boundingBox.html
.. _`condition`: tes3probe/condition.html
.. _`deleted`: tes3probe/deleted.html
.. _`disabled`: tes3probe/disabled.html
.. _`icon`: tes3probe/icon.html
.. _`id`: tes3probe/id.html
.. _`mesh`: tes3probe/mesh.html
.. _`modified`: tes3probe/modified.html
.. _`name`: tes3probe/name.html
.. _`nextInCollection`: tes3probe/nextInCollection.html
.. _`objectFlags`: tes3probe/objectFlags.html
.. _`objectType`: tes3probe/objectType.html
.. _`owningCollection`: tes3probe/owningCollection.html
.. _`previousInCollection`: tes3probe/previousInCollection.html
.. _`quality`: tes3probe/quality.html
.. _`scale`: tes3probe/scale.html
.. _`sceneNode`: tes3probe/sceneNode.html
.. _`sceneReference`: tes3probe/sceneReference.html
.. _`script`: tes3probe/script.html
.. _`sourceMod`: tes3probe/sourceMod.html
.. _`stolenList`: tes3probe/stolenList.html
.. _`supportsLuaData`: tes3probe/supportsLuaData.html
.. _`value`: tes3probe/value.html
.. _`weight`: tes3probe/weight.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
