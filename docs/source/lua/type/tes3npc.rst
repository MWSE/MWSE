tes3npc
====================================================================================================

An NPC object that has not been cloned. Typically represents the raw information edited in the construction set.

Properties
----------------------------------------------------------------------------------------------------

`actorFlags`_ (`number`_)
    Read-only. A number representing the actor flags. Truly a bit field.

`aiConfig`_ (`tes3aiConfig`_)
    Read-only. A substructure off of actors that contains information on the current AI configuration.

`attributes`_ (`table`_)
    A table of eight numbers, representing the base values for the actor's attributes.

`autoCalc`_ (`boolean`_)
    Direct access to the actor autocalc flag.

`barterGold`_ (`number`_)
    Read-only. The actor's max health.

`baseDisposition`_ (`number`_)
    The actor's base disposition.

`boundingBox`_ (`tes3boundingBox`_)
    The bounding box for the object.

`class`_ (`tes3class`_)
    Read-only. The class that the NPC uses.

`cloneCount`_ (`number`_)
    Read-only. The number of clones that exist of this actor.

`deleted`_ (`boolean`_)
    The deleted state of the object.

`disabled`_ (`boolean`_)
    The disabled state of the object.

`equipment`_ (`tes3iterator`_)
    Read-only. The items currently equipped to the actor.

`faction`_ (`tes3faction`_)
    Read-only. The class that the NPC is joined to.

`factionIndex`_ (`number`_)
    No description available.

`factionRank`_ (`number`_)
    The NPC's rank in their faction.

`fatigue`_ (`number`_)
    The actor's max fatigue.

`female`_ (`boolean`_)
    Direct access to the actor female flag.

`hair`_ (`tes3bodyPart`_)
    The hair body part that the NPC will use.

`head`_ (`tes3bodyPart`_)
    The head body part that the NPC will use.

`health`_ (`number`_)
    The actor's max health.

`id`_ (`string`_)
    The unique identifier for the object.

`inventory`_ (`tes3iterator`_)
    Read-only. The items currently carried by the actor.

`isAttacked`_ (`boolean`_)
    Read-only. If true, the actor's attacked flag is set.

`isEssential`_ (`boolean`_)
    Read-only. If true, the actor's essential flag is set.

`isInstance`_ (`boolean`_)
    Always returns false.

`isRespawn`_ (`boolean`_)
    Read-only. If true, the actor's respawn flag is set.

`level`_ (`number`_)
    The actor's level.

`magicka`_ (`number`_)
    The actor's max magicka.

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

`race`_ (`tes3race`_)
    Read-only. The race that the NPC uses.

`reputation`_ (`number`_)
    The actor's base reputation.

`scale`_ (`number`_)
    The object's scale.

`sceneNode`_ (`niNode`_)
    The scene graph node for this object.

`sceneReference`_ (`niNode`_)
    The scene graph reference node for this object.

`script`_ (`tes3script`_)
    The script that runs on the object.

`skills`_ (`table`_)
    A table of twenty seven numbers, representing the base values for the NPC's skills.

`sourceMod`_ (`string`_)
    The filename of the mod that owns this object.

`spells`_ (`tes3spellList`_)
    Read-only. A list of spells that the actor has access to. It is a tes3spellList, which is a list wrapper with helper functions. The actual list is accessed with .iterator. e.g. for _, spell in pairs(npc.spells.iterator) do print(spell.name) end

`stolenList`_ (`tes3iterator`_)
    A list of actors that the object has been stolen from.

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

.. toctree::
    :hidden:

    tes3npc/actorFlags
    tes3npc/aiConfig
    tes3npc/attributes
    tes3npc/autoCalc
    tes3npc/barterGold
    tes3npc/baseDisposition
    tes3npc/boundingBox
    tes3npc/class
    tes3npc/cloneCount
    tes3npc/deleted
    tes3npc/disabled
    tes3npc/equipment
    tes3npc/faction
    tes3npc/factionIndex
    tes3npc/factionRank
    tes3npc/fatigue
    tes3npc/female
    tes3npc/hair
    tes3npc/head
    tes3npc/health
    tes3npc/id
    tes3npc/inventory
    tes3npc/isAttacked
    tes3npc/isEssential
    tes3npc/isInstance
    tes3npc/isRespawn
    tes3npc/level
    tes3npc/magicka
    tes3npc/mesh
    tes3npc/modified
    tes3npc/name
    tes3npc/nextInCollection
    tes3npc/objectFlags
    tes3npc/objectType
    tes3npc/owningCollection
    tes3npc/previousInCollection
    tes3npc/race
    tes3npc/reputation
    tes3npc/scale
    tes3npc/sceneNode
    tes3npc/sceneReference
    tes3npc/script
    tes3npc/skills
    tes3npc/sourceMod
    tes3npc/spells
    tes3npc/stolenList
    tes3npc/supportsLuaData

.. _`actorFlags`: tes3npc/actorFlags.html
.. _`aiConfig`: tes3npc/aiConfig.html
.. _`attributes`: tes3npc/attributes.html
.. _`autoCalc`: tes3npc/autoCalc.html
.. _`barterGold`: tes3npc/barterGold.html
.. _`baseDisposition`: tes3npc/baseDisposition.html
.. _`boundingBox`: tes3npc/boundingBox.html
.. _`class`: tes3npc/class.html
.. _`cloneCount`: tes3npc/cloneCount.html
.. _`deleted`: tes3npc/deleted.html
.. _`disabled`: tes3npc/disabled.html
.. _`equipment`: tes3npc/equipment.html
.. _`faction`: tes3npc/faction.html
.. _`factionIndex`: tes3npc/factionIndex.html
.. _`factionRank`: tes3npc/factionRank.html
.. _`fatigue`: tes3npc/fatigue.html
.. _`female`: tes3npc/female.html
.. _`hair`: tes3npc/hair.html
.. _`head`: tes3npc/head.html
.. _`health`: tes3npc/health.html
.. _`id`: tes3npc/id.html
.. _`inventory`: tes3npc/inventory.html
.. _`isAttacked`: tes3npc/isAttacked.html
.. _`isEssential`: tes3npc/isEssential.html
.. _`isInstance`: tes3npc/isInstance.html
.. _`isRespawn`: tes3npc/isRespawn.html
.. _`level`: tes3npc/level.html
.. _`magicka`: tes3npc/magicka.html
.. _`mesh`: tes3npc/mesh.html
.. _`modified`: tes3npc/modified.html
.. _`name`: tes3npc/name.html
.. _`nextInCollection`: tes3npc/nextInCollection.html
.. _`objectFlags`: tes3npc/objectFlags.html
.. _`objectType`: tes3npc/objectType.html
.. _`owningCollection`: tes3npc/owningCollection.html
.. _`previousInCollection`: tes3npc/previousInCollection.html
.. _`race`: tes3npc/race.html
.. _`reputation`: tes3npc/reputation.html
.. _`scale`: tes3npc/scale.html
.. _`sceneNode`: tes3npc/sceneNode.html
.. _`sceneReference`: tes3npc/sceneReference.html
.. _`script`: tes3npc/script.html
.. _`skills`: tes3npc/skills.html
.. _`sourceMod`: tes3npc/sourceMod.html
.. _`spells`: tes3npc/spells.html
.. _`stolenList`: tes3npc/stolenList.html
.. _`supportsLuaData`: tes3npc/supportsLuaData.html

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

    tes3npc/offersService
    tes3npc/onInventoryClose
    tes3npc/tradesItemType

.. _`offersService`: tes3npc/offersService.html
.. _`onInventoryClose`: tes3npc/onInventoryClose.html
.. _`tradesItemType`: tes3npc/tradesItemType.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3bodyPart`: ../../lua/type/tes3bodyPart.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3class`: ../../lua/type/tes3class.html
.. _`tes3faction`: ../../lua/type/tes3faction.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3race`: ../../lua/type/tes3race.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
.. _`tes3spellList`: ../../lua/type/tes3spellList.html
