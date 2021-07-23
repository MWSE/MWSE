tes3armor
====================================================================================================

An armor game object.

Properties
----------------------------------------------------------------------------------------------------

`armorRating`_ (`number`_)
    The armor's defensive rating.

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

`isLeftPart`_ (`boolean`_)
    Determines if the armor is the left part of a pair.

`maxCondition`_ (`number`_)
    The object's maximum condition.

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

`parts`_ (`table`_)
    An array-style table of the tes3wearablePart data on the object.

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

`slot`_ (`number`_)
    The slot used by the armor.

`slotName`_ (`string`_)
    The name of the slot used by the armor.

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

`weightClass`_ (`number`_)
    The weight class of the armor.

.. toctree::
    :hidden:

    tes3armor/armorRating
    tes3armor/boundingBox
    tes3armor/deleted
    tes3armor/disabled
    tes3armor/enchantCapacity
    tes3armor/enchantment
    tes3armor/icon
    tes3armor/id
    tes3armor/isLeftPart
    tes3armor/maxCondition
    tes3armor/mesh
    tes3armor/modified
    tes3armor/name
    tes3armor/nextInCollection
    tes3armor/objectFlags
    tes3armor/objectType
    tes3armor/owningCollection
    tes3armor/parts
    tes3armor/previousInCollection
    tes3armor/scale
    tes3armor/sceneNode
    tes3armor/sceneReference
    tes3armor/script
    tes3armor/slot
    tes3armor/slotName
    tes3armor/sourceMod
    tes3armor/stolenList
    tes3armor/supportsLuaData
    tes3armor/value
    tes3armor/weight
    tes3armor/weightClass

.. _`armorRating`: tes3armor/armorRating.html
.. _`boundingBox`: tes3armor/boundingBox.html
.. _`deleted`: tes3armor/deleted.html
.. _`disabled`: tes3armor/disabled.html
.. _`enchantCapacity`: tes3armor/enchantCapacity.html
.. _`enchantment`: tes3armor/enchantment.html
.. _`icon`: tes3armor/icon.html
.. _`id`: tes3armor/id.html
.. _`isLeftPart`: tes3armor/isLeftPart.html
.. _`maxCondition`: tes3armor/maxCondition.html
.. _`mesh`: tes3armor/mesh.html
.. _`modified`: tes3armor/modified.html
.. _`name`: tes3armor/name.html
.. _`nextInCollection`: tes3armor/nextInCollection.html
.. _`objectFlags`: tes3armor/objectFlags.html
.. _`objectType`: tes3armor/objectType.html
.. _`owningCollection`: tes3armor/owningCollection.html
.. _`parts`: tes3armor/parts.html
.. _`previousInCollection`: tes3armor/previousInCollection.html
.. _`scale`: tes3armor/scale.html
.. _`sceneNode`: tes3armor/sceneNode.html
.. _`sceneReference`: tes3armor/sceneReference.html
.. _`script`: tes3armor/script.html
.. _`slot`: tes3armor/slot.html
.. _`slotName`: tes3armor/slotName.html
.. _`sourceMod`: tes3armor/sourceMod.html
.. _`stolenList`: tes3armor/stolenList.html
.. _`supportsLuaData`: tes3armor/supportsLuaData.html
.. _`value`: tes3armor/value.html
.. _`weight`: tes3armor/weight.html
.. _`weightClass`: tes3armor/weightClass.html

Methods
----------------------------------------------------------------------------------------------------

`calculateArmorRating`_
    Calculates what armor rating is provided for a given mobile actor.

.. toctree::
    :hidden:

    tes3armor/calculateArmorRating

.. _`calculateArmorRating`: tes3armor/calculateArmorRating.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3enchantment`: ../../lua/type/tes3enchantment.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
