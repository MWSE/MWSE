tes3container
====================================================================================================

A container object that has not been cloned. Typically represents the raw information edited in the construction set.

Properties
----------------------------------------------------------------------------------------------------

`actorFlags`_ (`number`_)
    Read-only. A number representing the actor flags. Truly a bit field.

`boundingBox`_ (`tes3boundingBox`_)
    The bounding box for the object.

`capacity`_ (`number`_)
    The amount of weight that the container can hold.

`cloneCount`_ (`number`_)
    Read-only. The number of clones that exist of this actor.

`deleted`_ (`boolean`_)
    The deleted state of the object.

`disabled`_ (`boolean`_)
    The disabled state of the object.

`equipment`_ (`tes3iterator`_)
    Read-only. The items currently equipped to the actor.

`id`_ (`string`_)
    The unique identifier for the object.

`inventory`_ (`tes3iterator`_)
    Read-only. The items currently carried by the actor.

`isInstance`_ (`boolean`_)
    Always returns false.

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

`organic`_ (`boolean`_)
    Determines if the container's organic flag is enabled.

`owningCollection`_ (`tes3referenceList`_)
    The collection responsible for holding this object.

`previousInCollection`_ (`tes3object`_)
    The previous object in parent collection's list.

`respawns`_ (`boolean`_)
    Determines if the container's respawn flag is enabled.

`scale`_ (`number`_)
    The object's scale.

`sceneNode`_ (`niNode`_)
    The scene graph node for this object.

`sceneReference`_ (`niNode`_)
    The scene graph reference node for this object.

`script`_ (`tes3script`_)
    The script that runs on the object.

`sourceMod`_ (`string`_)
    The filename of the mod that owns this object.

`stolenList`_ (`tes3iterator`_)
    A list of actors that the object has been stolen from.

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

.. toctree::
    :hidden:

    tes3container/actorFlags
    tes3container/boundingBox
    tes3container/capacity
    tes3container/cloneCount
    tes3container/deleted
    tes3container/disabled
    tes3container/equipment
    tes3container/id
    tes3container/inventory
    tes3container/isInstance
    tes3container/mesh
    tes3container/modified
    tes3container/name
    tes3container/nextInCollection
    tes3container/objectFlags
    tes3container/objectType
    tes3container/organic
    tes3container/owningCollection
    tes3container/previousInCollection
    tes3container/respawns
    tes3container/scale
    tes3container/sceneNode
    tes3container/sceneReference
    tes3container/script
    tes3container/sourceMod
    tes3container/stolenList
    tes3container/supportsLuaData

.. _`actorFlags`: tes3container/actorFlags.html
.. _`boundingBox`: tes3container/boundingBox.html
.. _`capacity`: tes3container/capacity.html
.. _`cloneCount`: tes3container/cloneCount.html
.. _`deleted`: tes3container/deleted.html
.. _`disabled`: tes3container/disabled.html
.. _`equipment`: tes3container/equipment.html
.. _`id`: tes3container/id.html
.. _`inventory`: tes3container/inventory.html
.. _`isInstance`: tes3container/isInstance.html
.. _`mesh`: tes3container/mesh.html
.. _`modified`: tes3container/modified.html
.. _`name`: tes3container/name.html
.. _`nextInCollection`: tes3container/nextInCollection.html
.. _`objectFlags`: tes3container/objectFlags.html
.. _`objectType`: tes3container/objectType.html
.. _`organic`: tes3container/organic.html
.. _`owningCollection`: tes3container/owningCollection.html
.. _`previousInCollection`: tes3container/previousInCollection.html
.. _`respawns`: tes3container/respawns.html
.. _`scale`: tes3container/scale.html
.. _`sceneNode`: tes3container/sceneNode.html
.. _`sceneReference`: tes3container/sceneReference.html
.. _`script`: tes3container/script.html
.. _`sourceMod`: tes3container/sourceMod.html
.. _`stolenList`: tes3container/stolenList.html
.. _`supportsLuaData`: tes3container/supportsLuaData.html

Methods
----------------------------------------------------------------------------------------------------

`offersService`_ (`boolean`_)
    Checks if the actor will offer a service in dialogue. This an offer and may still be refused by dialogue checks. To also get the result of dialogue checks, use tes3.checkMerchantOffersService.

`onInventoryClose`_
    A callback function invoked when an inventory is closed. Typically not used outside of specific purposes. You may find tes3.reference's onCloseInventory() to be more convenient to use.

`tradesItemType`_ (`boolean`_)
    Checks if the actor will buy and sell items of a given object type. e.g. actor:tradesItemType(tes3.objectType.repairItem)

.. toctree::
    :hidden:

    tes3container/offersService
    tes3container/onInventoryClose
    tes3container/tradesItemType

.. _`offersService`: tes3container/offersService.html
.. _`onInventoryClose`: tes3container/onInventoryClose.html
.. _`tradesItemType`: tes3container/tradesItemType.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
