tes3misc
====================================================================================================

A core miscellaneous object.

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

`icon`_ (`string`_)
    The path to the object's icon.

`id`_ (`string`_)
    Read-only. The unique identifier for the object.

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
    Read-only. The script that runs on the object.

`soulGemCapacity`_ (`number`_)
    Read-only. Fetches capacity of the soul gem, if this item is a soul gem.

`soulGemData`_ (`tes3soulGemData`_)
    Read-only. Fetches related soul gem data, if this item is a soul gem.

`sourceMod`_ (`string`_)
    Read-only. The filename of the mod that owns this object.

`sourceless`_ (`boolean`_)
    The soruceless flag of the object.

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

    tes3misc/blocked
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
    tes3misc/persistent
    tes3misc/previousInCollection
    tes3misc/scale
    tes3misc/sceneNode
    tes3misc/sceneReference
    tes3misc/script
    tes3misc/soulGemCapacity
    tes3misc/soulGemData
    tes3misc/sourceMod
    tes3misc/sourceless
    tes3misc/stolenList
    tes3misc/supportsLuaData
    tes3misc/value
    tes3misc/weight

.. _`blocked`: tes3misc/blocked.html
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
.. _`persistent`: tes3misc/persistent.html
.. _`previousInCollection`: tes3misc/previousInCollection.html
.. _`scale`: tes3misc/scale.html
.. _`sceneNode`: tes3misc/sceneNode.html
.. _`sceneReference`: tes3misc/sceneReference.html
.. _`script`: tes3misc/script.html
.. _`soulGemCapacity`: tes3misc/soulGemCapacity.html
.. _`soulGemData`: tes3misc/soulGemData.html
.. _`sourceMod`: tes3misc/sourceMod.html
.. _`sourceless`: tes3misc/sourceless.html
.. _`stolenList`: tes3misc/stolenList.html
.. _`supportsLuaData`: tes3misc/supportsLuaData.html
.. _`value`: tes3misc/value.html
.. _`weight`: tes3misc/weight.html

Methods
----------------------------------------------------------------------------------------------------

`__tojson`_ (`string`_)
    Serializes the object to json.

`isGold`_ (`boolean`_)
    Returns true if the misc item is gold.

.. toctree::
    :hidden:

    tes3misc/__tojson
    tes3misc/isGold

.. _`__tojson`: tes3misc/__tojson.html
.. _`isGold`: tes3misc/isGold.html

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
