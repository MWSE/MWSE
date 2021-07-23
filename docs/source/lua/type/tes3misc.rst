tes3misc
====================================================================================================

A core miscellaneous object.

Properties
----------------------------------------------------------------------------------------------------

`boundingBox`_ (`tes3boundingBox`_)
    The bounding box for the object.

`deleted`_ (`boolean`_)
    The deleted state of the object.

`disabled`_ (`boolean`_)
    The disabled state of the object.

`icon`_ (`string`_)
    The path to the object's icon.

`id`_ (`string`_)
    The unique identifier for the object.

`isKey`_ (`boolean`_)
    Access to the flag determining if this item is recognized as a key.

`isSoulGem`_ (`boolean`_)
    Read-only. Determines if this item is a soul gem.

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

`scale`_ (`number`_)
    The object's scale.

`sceneNode`_ (`niNode`_)
    The scene graph node for this object.

`sceneReference`_ (`niNode`_)
    The scene graph reference node for this object.

`script`_ (`tes3script`_)
    Read-only. The script that runs on the object.

`soulGemData`_ (`tes3soulGemData`_)
    Read-only. Fetches related soul gem data, if this item is a soul gem.

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

    tes3misc/boundingBox
    tes3misc/deleted
    tes3misc/disabled
    tes3misc/icon
    tes3misc/id
    tes3misc/isKey
    tes3misc/isSoulGem
    tes3misc/mesh
    tes3misc/modified
    tes3misc/name
    tes3misc/nextInCollection
    tes3misc/objectFlags
    tes3misc/objectType
    tes3misc/owningCollection
    tes3misc/previousInCollection
    tes3misc/scale
    tes3misc/sceneNode
    tes3misc/sceneReference
    tes3misc/script
    tes3misc/soulGemData
    tes3misc/sourceMod
    tes3misc/stolenList
    tes3misc/supportsLuaData
    tes3misc/value
    tes3misc/weight

.. _`boundingBox`: tes3misc/boundingBox.html
.. _`deleted`: tes3misc/deleted.html
.. _`disabled`: tes3misc/disabled.html
.. _`icon`: tes3misc/icon.html
.. _`id`: tes3misc/id.html
.. _`isKey`: tes3misc/isKey.html
.. _`isSoulGem`: tes3misc/isSoulGem.html
.. _`mesh`: tes3misc/mesh.html
.. _`modified`: tes3misc/modified.html
.. _`name`: tes3misc/name.html
.. _`nextInCollection`: tes3misc/nextInCollection.html
.. _`objectFlags`: tes3misc/objectFlags.html
.. _`objectType`: tes3misc/objectType.html
.. _`owningCollection`: tes3misc/owningCollection.html
.. _`previousInCollection`: tes3misc/previousInCollection.html
.. _`scale`: tes3misc/scale.html
.. _`sceneNode`: tes3misc/sceneNode.html
.. _`sceneReference`: tes3misc/sceneReference.html
.. _`script`: tes3misc/script.html
.. _`soulGemData`: tes3misc/soulGemData.html
.. _`sourceMod`: tes3misc/sourceMod.html
.. _`stolenList`: tes3misc/stolenList.html
.. _`supportsLuaData`: tes3misc/supportsLuaData.html
.. _`value`: tes3misc/value.html
.. _`weight`: tes3misc/weight.html

Functions
----------------------------------------------------------------------------------------------------

`create`_
    Creates a new misc item object, which will be stored as part of the current saved game.

.. toctree::
    :hidden:

    tes3misc/create

.. _`create`: tes3misc/create.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
.. _`tes3soulGemData`: ../../lua/type/tes3soulGemData.html
