tes3npcInstance
====================================================================================================

An NPC object that has been cloned. Typically represents an NPC that has been instanced in the world.

Properties
----------------------------------------------------------------------------------------------------

`actorFlags`_ (`number`_)
    Read-only. A number representing the actor flags. Truly a bit field.

`aiConfig`_ (`tes3aiConfig`_)
    Read-only. A substructure off of actors that contains information on the current AI configuration.

`attributes`_ (`table`_)
    Quick access to the base NPC's attributes.

`barterGold`_ (`number`_)
    Quick access to the base NPC's base amount of barter gold.

`baseObject`_ (`tes3npc`_)
    Read-only. Access to the base NPC object.

`boundingBox`_ (`tes3boundingBox`_)
    The bounding box for the object.

`class`_ (`tes3class`_)
    Read-only. Quick access to the base NPC's class.

`cloneCount`_ (`number`_)
    Read-only. The number of clones that exist of this actor.

`deleted`_ (`boolean`_)
    The deleted state of the object.

`disabled`_ (`boolean`_)
    The disabled state of the object.

`disposition`_ (`number`_)
    Read-only. The actor's effective disposition, with all modifiers applied.

`equipment`_ (`tes3iterator`_)
    Read-only. The items currently equipped to the actor.

`faction`_ (`tes3faction`_)
    Read-only. Quick access to the base NPC's faction.

`factionIndex`_ (`number`_)
    No description available.

`fatigue`_ (`number`_)
    Read-only. Quick access to the base NPC's fatigue.

`health`_ (`number`_)
    Read-only. Quick access to the base NPC's health.

`id`_ (`string`_)
    The unique identifier for the object.

`inventory`_ (`tes3iterator`_)
    Read-only. The items currently carried by the actor.

`isAttacked`_ (`boolean`_)
    Read-only. If true, the actor's attacked flag is set.

`isEssential`_ (`boolean`_)
    Read-only. If true, the actor's essential flag is set.

`isInstance`_ (`boolean`_)
    Always returns true.

`isRespawn`_ (`boolean`_)
    Read-only. If true, the actor's respawn flag is set.

`level`_ (`number`_)
    Read-only. Quick access to the base NPC's level.

`magicka`_ (`number`_)
    Read-only. Quick access to the base NPC's magicka.

`modified`_ (`boolean`_)
    The modification state of the object since the last save.

`name`_ (`number`_)
    Quick access to the base NPC's name.

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

`race`_ (`tes3race`_)
    Read-only. Quick access to the base NPC's race.

`scale`_ (`number`_)
    The object's scale.

`sceneNode`_ (`niNode`_)
    The scene graph node for this object.

`sceneReference`_ (`niNode`_)
    The scene graph reference node for this object.

`script`_ (`tes3script`_)
    Read-only. Quick access to the base NPC's script.

`skills`_ (`table`_)
    Quick access to the base NPC's skills.

`sourceMod`_ (`string`_)
    The filename of the mod that owns this object.

`spells`_ (`tes3spellList`_)
    Read-only. Quick access to the base NPC's spell list. It is a tes3spellList, which is a list wrapper with helper functions. The actual list is accessed with .iterator. e.g. for _, spell in pairs(npc.spells.iterator) do print(spell.name) end

`stolenList`_ (`tes3iterator`_)
    A list of actors that the object has been stolen from.

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

.. toctree::
    :hidden:

    tes3npcInstance/actorFlags
    tes3npcInstance/aiConfig
    tes3npcInstance/attributes
    tes3npcInstance/barterGold
    tes3npcInstance/baseObject
    tes3npcInstance/boundingBox
    tes3npcInstance/class
    tes3npcInstance/cloneCount
    tes3npcInstance/deleted
    tes3npcInstance/disabled
    tes3npcInstance/disposition
    tes3npcInstance/equipment
    tes3npcInstance/faction
    tes3npcInstance/factionIndex
    tes3npcInstance/fatigue
    tes3npcInstance/health
    tes3npcInstance/id
    tes3npcInstance/inventory
    tes3npcInstance/isAttacked
    tes3npcInstance/isEssential
    tes3npcInstance/isInstance
    tes3npcInstance/isRespawn
    tes3npcInstance/level
    tes3npcInstance/magicka
    tes3npcInstance/modified
    tes3npcInstance/name
    tes3npcInstance/nextInCollection
    tes3npcInstance/objectFlags
    tes3npcInstance/objectType
    tes3npcInstance/owningCollection
    tes3npcInstance/previousInCollection
    tes3npcInstance/race
    tes3npcInstance/scale
    tes3npcInstance/sceneNode
    tes3npcInstance/sceneReference
    tes3npcInstance/script
    tes3npcInstance/skills
    tes3npcInstance/sourceMod
    tes3npcInstance/spells
    tes3npcInstance/stolenList
    tes3npcInstance/supportsLuaData

.. _`actorFlags`: tes3npcInstance/actorFlags.html
.. _`aiConfig`: tes3npcInstance/aiConfig.html
.. _`attributes`: tes3npcInstance/attributes.html
.. _`barterGold`: tes3npcInstance/barterGold.html
.. _`baseObject`: tes3npcInstance/baseObject.html
.. _`boundingBox`: tes3npcInstance/boundingBox.html
.. _`class`: tes3npcInstance/class.html
.. _`cloneCount`: tes3npcInstance/cloneCount.html
.. _`deleted`: tes3npcInstance/deleted.html
.. _`disabled`: tes3npcInstance/disabled.html
.. _`disposition`: tes3npcInstance/disposition.html
.. _`equipment`: tes3npcInstance/equipment.html
.. _`faction`: tes3npcInstance/faction.html
.. _`factionIndex`: tes3npcInstance/factionIndex.html
.. _`fatigue`: tes3npcInstance/fatigue.html
.. _`health`: tes3npcInstance/health.html
.. _`id`: tes3npcInstance/id.html
.. _`inventory`: tes3npcInstance/inventory.html
.. _`isAttacked`: tes3npcInstance/isAttacked.html
.. _`isEssential`: tes3npcInstance/isEssential.html
.. _`isInstance`: tes3npcInstance/isInstance.html
.. _`isRespawn`: tes3npcInstance/isRespawn.html
.. _`level`: tes3npcInstance/level.html
.. _`magicka`: tes3npcInstance/magicka.html
.. _`modified`: tes3npcInstance/modified.html
.. _`name`: tes3npcInstance/name.html
.. _`nextInCollection`: tes3npcInstance/nextInCollection.html
.. _`objectFlags`: tes3npcInstance/objectFlags.html
.. _`objectType`: tes3npcInstance/objectType.html
.. _`owningCollection`: tes3npcInstance/owningCollection.html
.. _`previousInCollection`: tes3npcInstance/previousInCollection.html
.. _`race`: tes3npcInstance/race.html
.. _`scale`: tes3npcInstance/scale.html
.. _`sceneNode`: tes3npcInstance/sceneNode.html
.. _`sceneReference`: tes3npcInstance/sceneReference.html
.. _`script`: tes3npcInstance/script.html
.. _`skills`: tes3npcInstance/skills.html
.. _`sourceMod`: tes3npcInstance/sourceMod.html
.. _`spells`: tes3npcInstance/spells.html
.. _`stolenList`: tes3npcInstance/stolenList.html
.. _`supportsLuaData`: tes3npcInstance/supportsLuaData.html

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

    tes3npcInstance/offersService
    tes3npcInstance/onInventoryClose
    tes3npcInstance/tradesItemType

.. _`offersService`: tes3npcInstance/offersService.html
.. _`onInventoryClose`: tes3npcInstance/onInventoryClose.html
.. _`tradesItemType`: tes3npcInstance/tradesItemType.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3class`: ../../lua/type/tes3class.html
.. _`tes3faction`: ../../lua/type/tes3faction.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3npc`: ../../lua/type/tes3npc.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3race`: ../../lua/type/tes3race.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
.. _`tes3spellList`: ../../lua/type/tes3spellList.html
