tes3creatureInstance
====================================================================================================

A creature object that has been cloned. Typically represents a creature that has been instanced in the world.

Properties
----------------------------------------------------------------------------------------------------

`actorFlags`_ (`number`_)
    Read-only. A number representing the actor flags. Truly a bit field.

`aiConfig`_ (`tes3aiConfig`_)
    Read-only. Simplified access to the base creature's AI configuration.

`attacks`_ (`table`_)
    Simplified access to the base creature's attacks. A table of three attacks, represented by a trio of tes3rangeInt.

`attributes`_ (`table`_)
    Simplified access to the base creature's attributes. A table of eight numbers, representing the base values for the creature's attributes.

`barterGold`_ (`number`_)
    The amount of gold that the creature has to barter with.

`baseObject`_ (`tes3creature`_)
    Access to creature that this one is instanced from.

`biped`_ (`boolean`_)
    Access to the creature's biped flag.

`boundingBox`_ (`tes3boundingBox`_)
    The bounding box for the object.

`cloneCount`_ (`number`_)
    Read-only. The number of clones that exist of this actor.

`deleted`_ (`boolean`_)
    The deleted state of the object.

`disabled`_ (`boolean`_)
    The disabled state of the object.

`equipment`_ (`tes3iterator`_)
    Read-only. A collection that contains the currently equipped items.

`equipment`_ (`tes3iterator`_)
    Read-only. The items currently equipped to the actor.

`fatigue`_ (`number`_)
    Read-only. The creature's current fatigue.

`flies`_ (`boolean`_)
    Access to the creature's flies flag.

`health`_ (`number`_)
    Read-only. The creature's current health.

`id`_ (`string`_)
    The unique identifier for the object.

`inventory`_ (`tes3iterator`_)
    Read-only. The items currently carried by the actor.

`inventory`_ (`tes3iterator`_)
    Read-only. A collection that contains the items in the actor's inventory.

`isAttacked`_ (`boolean`_)
    Read-only. If true, the creature attacked flag is set.

`isEssential`_ (`boolean`_)
    Read-only. If true, the creature essential flag is set.

`isInstance`_ (`boolean`_)
    Always returns true.

`isRespawn`_ (`boolean`_)
    Read-only. If true, the creature respawn flag is set.

`level`_ (`number`_)
    Read-only. The base level of the creature.

`magicka`_ (`number`_)
    Read-only. The creature's current magicka.

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

`respawns`_ (`boolean`_)
    Access to the creature's respawns flag.

`scale`_ (`number`_)
    The object's scale.

`sceneNode`_ (`niNode`_)
    The scene graph node for this object.

`sceneReference`_ (`niNode`_)
    The scene graph reference node for this object.

`script`_ (`tes3script`_)
    The script that runs on the object.

`skills`_ (`table`_)
    Simplified access to the base creature's skills. A table of three numbers, representing the base values for the creature's combat, magic, and stealth skills.

`soul`_ (`number`_)
    Simplified access to the base creature's soul. The amount of soul value that the creature provides.

`soundCreature`_ (`tes3creature`_)
    Simplified access to the base creature's sound generator. A creature to use instead of this one for sound generation.

`sourceMod`_ (`string`_)
    The filename of the mod that owns this object.

`spells`_ (`tes3spellList`_)
    Read-only. Quick access to the base creature's spell list. It is a tes3spellList, which is a list wrapper with helper functions. The actual list is accessed with .iterator. e.g. for _, spell in pairs(creature.spells.iterator) do print(spell.name) end

`stolenList`_ (`tes3iterator`_)
    A list of actors that the object has been stolen from.

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`swims`_ (`boolean`_)
    Access to the creature's swims flag.

`type`_ (`number`_)
    Simplified access to the base creature's type. The type of the creature, represented by a number for normal, daedra, undead, or humanoid.

`usesEquipment`_ (`boolean`_)
    Access to the creature's usesEquipment flag.

`walks`_ (`boolean`_)
    Access to the creature's walks flag.

`weapon`_ (`tes3weapon`_)
    Read-only. The creature's currently equipped weapon.

.. toctree::
    :hidden:

    tes3creatureInstance/actorFlags
    tes3creatureInstance/aiConfig
    tes3creatureInstance/attacks
    tes3creatureInstance/attributes
    tes3creatureInstance/barterGold
    tes3creatureInstance/baseObject
    tes3creatureInstance/biped
    tes3creatureInstance/boundingBox
    tes3creatureInstance/cloneCount
    tes3creatureInstance/deleted
    tes3creatureInstance/disabled
    tes3creatureInstance/equipment
    tes3creatureInstance/equipment
    tes3creatureInstance/fatigue
    tes3creatureInstance/flies
    tes3creatureInstance/health
    tes3creatureInstance/id
    tes3creatureInstance/inventory
    tes3creatureInstance/inventory
    tes3creatureInstance/isAttacked
    tes3creatureInstance/isEssential
    tes3creatureInstance/isInstance
    tes3creatureInstance/isRespawn
    tes3creatureInstance/level
    tes3creatureInstance/magicka
    tes3creatureInstance/mesh
    tes3creatureInstance/modified
    tes3creatureInstance/name
    tes3creatureInstance/nextInCollection
    tes3creatureInstance/objectFlags
    tes3creatureInstance/objectType
    tes3creatureInstance/owningCollection
    tes3creatureInstance/previousInCollection
    tes3creatureInstance/respawns
    tes3creatureInstance/scale
    tes3creatureInstance/sceneNode
    tes3creatureInstance/sceneReference
    tes3creatureInstance/script
    tes3creatureInstance/skills
    tes3creatureInstance/soul
    tes3creatureInstance/soundCreature
    tes3creatureInstance/sourceMod
    tes3creatureInstance/spells
    tes3creatureInstance/stolenList
    tes3creatureInstance/supportsLuaData
    tes3creatureInstance/swims
    tes3creatureInstance/type
    tes3creatureInstance/usesEquipment
    tes3creatureInstance/walks
    tes3creatureInstance/weapon

.. _`actorFlags`: tes3creatureInstance/actorFlags.html
.. _`aiConfig`: tes3creatureInstance/aiConfig.html
.. _`attacks`: tes3creatureInstance/attacks.html
.. _`attributes`: tes3creatureInstance/attributes.html
.. _`barterGold`: tes3creatureInstance/barterGold.html
.. _`baseObject`: tes3creatureInstance/baseObject.html
.. _`biped`: tes3creatureInstance/biped.html
.. _`boundingBox`: tes3creatureInstance/boundingBox.html
.. _`cloneCount`: tes3creatureInstance/cloneCount.html
.. _`deleted`: tes3creatureInstance/deleted.html
.. _`disabled`: tes3creatureInstance/disabled.html
.. _`equipment`: tes3creatureInstance/equipment.html
.. _`equipment`: tes3creatureInstance/equipment.html
.. _`fatigue`: tes3creatureInstance/fatigue.html
.. _`flies`: tes3creatureInstance/flies.html
.. _`health`: tes3creatureInstance/health.html
.. _`id`: tes3creatureInstance/id.html
.. _`inventory`: tes3creatureInstance/inventory.html
.. _`inventory`: tes3creatureInstance/inventory.html
.. _`isAttacked`: tes3creatureInstance/isAttacked.html
.. _`isEssential`: tes3creatureInstance/isEssential.html
.. _`isInstance`: tes3creatureInstance/isInstance.html
.. _`isRespawn`: tes3creatureInstance/isRespawn.html
.. _`level`: tes3creatureInstance/level.html
.. _`magicka`: tes3creatureInstance/magicka.html
.. _`mesh`: tes3creatureInstance/mesh.html
.. _`modified`: tes3creatureInstance/modified.html
.. _`name`: tes3creatureInstance/name.html
.. _`nextInCollection`: tes3creatureInstance/nextInCollection.html
.. _`objectFlags`: tes3creatureInstance/objectFlags.html
.. _`objectType`: tes3creatureInstance/objectType.html
.. _`owningCollection`: tes3creatureInstance/owningCollection.html
.. _`previousInCollection`: tes3creatureInstance/previousInCollection.html
.. _`respawns`: tes3creatureInstance/respawns.html
.. _`scale`: tes3creatureInstance/scale.html
.. _`sceneNode`: tes3creatureInstance/sceneNode.html
.. _`sceneReference`: tes3creatureInstance/sceneReference.html
.. _`script`: tes3creatureInstance/script.html
.. _`skills`: tes3creatureInstance/skills.html
.. _`soul`: tes3creatureInstance/soul.html
.. _`soundCreature`: tes3creatureInstance/soundCreature.html
.. _`sourceMod`: tes3creatureInstance/sourceMod.html
.. _`spells`: tes3creatureInstance/spells.html
.. _`stolenList`: tes3creatureInstance/stolenList.html
.. _`supportsLuaData`: tes3creatureInstance/supportsLuaData.html
.. _`swims`: tes3creatureInstance/swims.html
.. _`type`: tes3creatureInstance/type.html
.. _`usesEquipment`: tes3creatureInstance/usesEquipment.html
.. _`walks`: tes3creatureInstance/walks.html
.. _`weapon`: tes3creatureInstance/weapon.html

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

    tes3creatureInstance/offersService
    tes3creatureInstance/onInventoryClose
    tes3creatureInstance/tradesItemType

.. _`offersService`: tes3creatureInstance/offersService.html
.. _`onInventoryClose`: tes3creatureInstance/onInventoryClose.html
.. _`tradesItemType`: tes3creatureInstance/tradesItemType.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3creature`: ../../lua/type/tes3creature.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
.. _`tes3spellList`: ../../lua/type/tes3spellList.html
.. _`tes3weapon`: ../../lua/type/tes3weapon.html
