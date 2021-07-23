tes3containerInstance
====================================================================================================

A container object that has been cloned. Typically represents a container that has been instanced by being opened by the player.

Properties
----------------------------------------------------------------------------------------------------

`actorFlags`_ (`number`_)
    Read-only. A number representing the actor flags. Truly a bit field.

`baseObject`_ (`tes3object`_)
    Read-only. The base container object that the instance inherits from.

`boundingBox`_ (`tes3boundingBox`_)
    The bounding box for the object.

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
    Always returns true.

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

    tes3containerInstance/actorFlags
    tes3containerInstance/baseObject
    tes3containerInstance/boundingBox
    tes3containerInstance/cloneCount
    tes3containerInstance/deleted
    tes3containerInstance/disabled
    tes3containerInstance/equipment
    tes3containerInstance/id
    tes3containerInstance/inventory
    tes3containerInstance/isInstance
    tes3containerInstance/mesh
    tes3containerInstance/modified
    tes3containerInstance/name
    tes3containerInstance/nextInCollection
    tes3containerInstance/objectFlags
    tes3containerInstance/objectType
    tes3containerInstance/organic
    tes3containerInstance/owningCollection
    tes3containerInstance/previousInCollection
    tes3containerInstance/respawns
    tes3containerInstance/scale
    tes3containerInstance/sceneNode
    tes3containerInstance/sceneReference
    tes3containerInstance/script
    tes3containerInstance/sourceMod
    tes3containerInstance/stolenList
    tes3containerInstance/supportsLuaData

.. _`actorFlags`: tes3containerInstance/actorFlags.html
.. _`baseObject`: tes3containerInstance/baseObject.html
.. _`boundingBox`: tes3containerInstance/boundingBox.html
.. _`cloneCount`: tes3containerInstance/cloneCount.html
.. _`deleted`: tes3containerInstance/deleted.html
.. _`disabled`: tes3containerInstance/disabled.html
.. _`equipment`: tes3containerInstance/equipment.html
.. _`id`: tes3containerInstance/id.html
.. _`inventory`: tes3containerInstance/inventory.html
.. _`isInstance`: tes3containerInstance/isInstance.html
.. _`mesh`: tes3containerInstance/mesh.html
.. _`modified`: tes3containerInstance/modified.html
.. _`name`: tes3containerInstance/name.html
.. _`nextInCollection`: tes3containerInstance/nextInCollection.html
.. _`objectFlags`: tes3containerInstance/objectFlags.html
.. _`objectType`: tes3containerInstance/objectType.html
.. _`organic`: tes3containerInstance/organic.html
.. _`owningCollection`: tes3containerInstance/owningCollection.html
.. _`previousInCollection`: tes3containerInstance/previousInCollection.html
.. _`respawns`: tes3containerInstance/respawns.html
.. _`scale`: tes3containerInstance/scale.html
.. _`sceneNode`: tes3containerInstance/sceneNode.html
.. _`sceneReference`: tes3containerInstance/sceneReference.html
.. _`script`: tes3containerInstance/script.html
.. _`sourceMod`: tes3containerInstance/sourceMod.html
.. _`stolenList`: tes3containerInstance/stolenList.html
.. _`supportsLuaData`: tes3containerInstance/supportsLuaData.html

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

    tes3containerInstance/offersService
    tes3containerInstance/onInventoryClose
    tes3containerInstance/tradesItemType

.. _`offersService`: tes3containerInstance/offersService.html
.. _`onInventoryClose`: tes3containerInstance/onInventoryClose.html
.. _`tradesItemType`: tes3containerInstance/tradesItemType.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
