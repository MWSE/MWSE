tes3book
====================================================================================================

A book game object.

Properties
----------------------------------------------------------------------------------------------------

`boundingBox`_ (`tes3boundingBox`_)
    The bounding box for the object.

`deleted`_ (`boolean`_)
    The deleted state of the object.

`disabled`_ (`boolean`_)
    The disabled state of the object.

`enchantCapacity`_ (`number`_)
    The object's enchantment capacity.

`enchantment`_ (`tes3enchantment`_)
    The enchantment used by the object.

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

`scale`_ (`number`_)
    The object's scale.

`sceneNode`_ (`niNode`_)
    The scene graph node for this object.

`sceneReference`_ (`niNode`_)
    The scene graph reference node for this object.

`script`_ (`tes3script`_)
    The script that runs on the object.

`skill`_ (`number`_)
    The skill learned from the book, or -1 if the book doesn't have one, or has already been read.

`sourceMod`_ (`string`_)
    The filename of the mod that owns this object.

`stolenList`_ (`tes3iterator`_)
    A list of actors that the object has been stolen from.

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`text`_ (`string`_)
    Read-only. Loads and displays the text of the book.

`type`_ (`number`_)
    The book type, where 0 is book and 1 is scroll.

`value`_ (`number`_)
    The value of the object.

`weight`_ (`number`_)
    The weight of the object.

.. toctree::
    :hidden:

    tes3book/boundingBox
    tes3book/deleted
    tes3book/disabled
    tes3book/enchantCapacity
    tes3book/enchantment
    tes3book/icon
    tes3book/id
    tes3book/mesh
    tes3book/modified
    tes3book/name
    tes3book/nextInCollection
    tes3book/objectFlags
    tes3book/objectType
    tes3book/owningCollection
    tes3book/previousInCollection
    tes3book/scale
    tes3book/sceneNode
    tes3book/sceneReference
    tes3book/script
    tes3book/skill
    tes3book/sourceMod
    tes3book/stolenList
    tes3book/supportsLuaData
    tes3book/text
    tes3book/type
    tes3book/value
    tes3book/weight

.. _`boundingBox`: tes3book/boundingBox.html
.. _`deleted`: tes3book/deleted.html
.. _`disabled`: tes3book/disabled.html
.. _`enchantCapacity`: tes3book/enchantCapacity.html
.. _`enchantment`: tes3book/enchantment.html
.. _`icon`: tes3book/icon.html
.. _`id`: tes3book/id.html
.. _`mesh`: tes3book/mesh.html
.. _`modified`: tes3book/modified.html
.. _`name`: tes3book/name.html
.. _`nextInCollection`: tes3book/nextInCollection.html
.. _`objectFlags`: tes3book/objectFlags.html
.. _`objectType`: tes3book/objectType.html
.. _`owningCollection`: tes3book/owningCollection.html
.. _`previousInCollection`: tes3book/previousInCollection.html
.. _`scale`: tes3book/scale.html
.. _`sceneNode`: tes3book/sceneNode.html
.. _`sceneReference`: tes3book/sceneReference.html
.. _`script`: tes3book/script.html
.. _`skill`: tes3book/skill.html
.. _`sourceMod`: tes3book/sourceMod.html
.. _`stolenList`: tes3book/stolenList.html
.. _`supportsLuaData`: tes3book/supportsLuaData.html
.. _`text`: tes3book/text.html
.. _`type`: tes3book/type.html
.. _`value`: tes3book/value.html
.. _`weight`: tes3book/weight.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3enchantment`: ../../lua/type/tes3enchantment.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
