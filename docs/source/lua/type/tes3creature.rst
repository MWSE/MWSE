tes3creature
====================================================================================================

A creature object that has not been cloned. Typically represents the raw information edited in the construction set.

Properties
----------------------------------------------------------------------------------------------------

`actorFlags`_ (`number`_)
    Read-only. A number representing the actor flags. Truly a bit field.

`aiConfig`_ (`tes3aiConfig`_)
    Read-only. A substructure off of actors that contains information on the current AI configuration.

`attacks`_ (`table`_)
    A table of three attacks, represented by a trio of tes3rangeInt.

`attributes`_ (`table`_)
    A table of eight numbers, representing the base values for the actor's attributes.

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
    Read-only. The items currently equipped to the actor.

`fatigue`_ (`number`_)
    Read-only. The actor's max fatigue.

`flies`_ (`boolean`_)
    Access to the creature's flies flag.

`health`_ (`number`_)
    Read-only. The actor's max health.

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
    Read-only. The base level of the creature.

`magicka`_ (`number`_)
    Read-only. The actor's max magicka.

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
    A table of three numbers, representing the base values for the creature's combat, magic, and stealth skills.

`soul`_ (`number`_)
    The amount of soul value that the creature provides.

`soundCreature`_ (`tes3creature`_)
    A creature to use instead of this one for sound generation.

`sourceMod`_ (`string`_)
    The filename of the mod that owns this object.

`spells`_ (`tes3spellList`_)
    Read-only. A list of spells that the actor has access to. It is a tes3spellList, which is a list wrapper with helper functions. The actual list is accessed with .iterator. e.g. for _, spell in pairs(creature.spells.iterator) do print(spell.name) end

`stolenList`_ (`tes3iterator`_)
    A list of actors that the object has been stolen from.

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`swims`_ (`boolean`_)
    Access to the creature's swims flag.

`type`_ (`number`_)
    The type of the creature, represented by a number for normal, daedra, undead, or humanoid.

`usesEquipment`_ (`boolean`_)
    Access to the creature's usesEquipment flag.

`walks`_ (`boolean`_)
    Access to the creature's walks flag.

.. toctree::
    :hidden:

    tes3creature/actorFlags
    tes3creature/aiConfig
    tes3creature/attacks
    tes3creature/attributes
    tes3creature/biped
    tes3creature/boundingBox
    tes3creature/cloneCount
    tes3creature/deleted
    tes3creature/disabled
    tes3creature/equipment
    tes3creature/fatigue
    tes3creature/flies
    tes3creature/health
    tes3creature/id
    tes3creature/inventory
    tes3creature/isAttacked
    tes3creature/isEssential
    tes3creature/isInstance
    tes3creature/isRespawn
    tes3creature/level
    tes3creature/magicka
    tes3creature/mesh
    tes3creature/modified
    tes3creature/name
    tes3creature/nextInCollection
    tes3creature/objectFlags
    tes3creature/objectType
    tes3creature/owningCollection
    tes3creature/previousInCollection
    tes3creature/respawns
    tes3creature/scale
    tes3creature/sceneNode
    tes3creature/sceneReference
    tes3creature/script
    tes3creature/skills
    tes3creature/soul
    tes3creature/soundCreature
    tes3creature/sourceMod
    tes3creature/spells
    tes3creature/stolenList
    tes3creature/supportsLuaData
    tes3creature/swims
    tes3creature/type
    tes3creature/usesEquipment
    tes3creature/walks

.. _`actorFlags`: tes3creature/actorFlags.html
.. _`aiConfig`: tes3creature/aiConfig.html
.. _`attacks`: tes3creature/attacks.html
.. _`attributes`: tes3creature/attributes.html
.. _`biped`: tes3creature/biped.html
.. _`boundingBox`: tes3creature/boundingBox.html
.. _`cloneCount`: tes3creature/cloneCount.html
.. _`deleted`: tes3creature/deleted.html
.. _`disabled`: tes3creature/disabled.html
.. _`equipment`: tes3creature/equipment.html
.. _`fatigue`: tes3creature/fatigue.html
.. _`flies`: tes3creature/flies.html
.. _`health`: tes3creature/health.html
.. _`id`: tes3creature/id.html
.. _`inventory`: tes3creature/inventory.html
.. _`isAttacked`: tes3creature/isAttacked.html
.. _`isEssential`: tes3creature/isEssential.html
.. _`isInstance`: tes3creature/isInstance.html
.. _`isRespawn`: tes3creature/isRespawn.html
.. _`level`: tes3creature/level.html
.. _`magicka`: tes3creature/magicka.html
.. _`mesh`: tes3creature/mesh.html
.. _`modified`: tes3creature/modified.html
.. _`name`: tes3creature/name.html
.. _`nextInCollection`: tes3creature/nextInCollection.html
.. _`objectFlags`: tes3creature/objectFlags.html
.. _`objectType`: tes3creature/objectType.html
.. _`owningCollection`: tes3creature/owningCollection.html
.. _`previousInCollection`: tes3creature/previousInCollection.html
.. _`respawns`: tes3creature/respawns.html
.. _`scale`: tes3creature/scale.html
.. _`sceneNode`: tes3creature/sceneNode.html
.. _`sceneReference`: tes3creature/sceneReference.html
.. _`script`: tes3creature/script.html
.. _`skills`: tes3creature/skills.html
.. _`soul`: tes3creature/soul.html
.. _`soundCreature`: tes3creature/soundCreature.html
.. _`sourceMod`: tes3creature/sourceMod.html
.. _`spells`: tes3creature/spells.html
.. _`stolenList`: tes3creature/stolenList.html
.. _`supportsLuaData`: tes3creature/supportsLuaData.html
.. _`swims`: tes3creature/swims.html
.. _`type`: tes3creature/type.html
.. _`usesEquipment`: tes3creature/usesEquipment.html
.. _`walks`: tes3creature/walks.html

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

    tes3creature/offersService
    tes3creature/onInventoryClose
    tes3creature/tradesItemType

.. _`offersService`: tes3creature/offersService.html
.. _`onInventoryClose`: tes3creature/onInventoryClose.html
.. _`tradesItemType`: tes3creature/tradesItemType.html

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
