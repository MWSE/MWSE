tes3faction
====================================================================================================

A faction game object.

Properties
----------------------------------------------------------------------------------------------------

`actorFlags`_ (`number`_)
    A number representing the actor flags. Truly a bit field.

`attributes`_ (`table`_)
    An array-style table holding the two attributes that govern advancement.

`boundingBox`_ (`tes3boundingBox`_)
    The bounding box for the object.

`cloneCount`_ (`number`_)
    The number of clones that exist of this actor.

`deleted`_ (`boolean`_)
    The deleted state of the object.

`disabled`_ (`boolean`_)
    The disabled state of the object.

`equipment`_ (`tes3iterator`_)
    The items currently equipped to the actor.

`id`_ (`string`_)
    The unique identifier for the object.

`inventory`_ (`tes3iterator`_)
    The items currently carried by the actor.

`modified`_ (`boolean`_)
    The modification state of the object since the last save.

`name`_ (`string`_)
    The faction's player-facing name.

`nextInCollection`_ (`tes3object`_)
    The next object in parent collection's list.

`objectFlags`_ (`number`_)
    The raw flags of the object.

`objectType`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`owningCollection`_ (`tes3referenceList`_)
    The collection responsible for holding this object.

`playerExpelled`_ (`boolean`_)
    The player's expelled state in the faction.

`playerJoined`_ (`boolean`_)
    The player's join state for the faction.

`playerRank`_ (`number`_)
    The player's current rank in the faction.

`playerReputation`_ (`number`_)
    The player's current reputation in the faction.

`previousInCollection`_ (`tes3object`_)
    The previous object in parent collection's list.

`ranks`_ (`table`_)
    An array-style table holding the ten related tes3factionRanks.

`reactions`_ (`tes3iterator`_)
    A collection of tes3factionReactions.

`scale`_ (`number`_)
    The object's scale.

`sceneNode`_ (`niNode`_)
    The scene graph node for this object.

`sceneReference`_ (`niNode`_)
    The scene graph reference node for this object.

`skills`_ (`table`_)
    An array-style table holding the seven skills that govern advancement.

`sourceMod`_ (`string`_)
    The filename of the mod that owns this object.

`stolenList`_ (`tes3iterator`_)
    A list of actors that the object has been stolen from.

.. toctree::
    :hidden:

    tes3faction/actorFlags
    tes3faction/attributes
    tes3faction/boundingBox
    tes3faction/cloneCount
    tes3faction/deleted
    tes3faction/disabled
    tes3faction/equipment
    tes3faction/id
    tes3faction/inventory
    tes3faction/modified
    tes3faction/name
    tes3faction/nextInCollection
    tes3faction/objectFlags
    tes3faction/objectType
    tes3faction/owningCollection
    tes3faction/playerExpelled
    tes3faction/playerJoined
    tes3faction/playerRank
    tes3faction/playerReputation
    tes3faction/previousInCollection
    tes3faction/ranks
    tes3faction/reactions
    tes3faction/scale
    tes3faction/sceneNode
    tes3faction/sceneReference
    tes3faction/skills
    tes3faction/sourceMod
    tes3faction/stolenList

.. _`actorFlags`: tes3faction/actorFlags.html
.. _`attributes`: tes3faction/attributes.html
.. _`boundingBox`: tes3faction/boundingBox.html
.. _`cloneCount`: tes3faction/cloneCount.html
.. _`deleted`: tes3faction/deleted.html
.. _`disabled`: tes3faction/disabled.html
.. _`equipment`: tes3faction/equipment.html
.. _`id`: tes3faction/id.html
.. _`inventory`: tes3faction/inventory.html
.. _`modified`: tes3faction/modified.html
.. _`name`: tes3faction/name.html
.. _`nextInCollection`: tes3faction/nextInCollection.html
.. _`objectFlags`: tes3faction/objectFlags.html
.. _`objectType`: tes3faction/objectType.html
.. _`owningCollection`: tes3faction/owningCollection.html
.. _`playerExpelled`: tes3faction/playerExpelled.html
.. _`playerJoined`: tes3faction/playerJoined.html
.. _`playerRank`: tes3faction/playerRank.html
.. _`playerReputation`: tes3faction/playerReputation.html
.. _`previousInCollection`: tes3faction/previousInCollection.html
.. _`ranks`: tes3faction/ranks.html
.. _`reactions`: tes3faction/reactions.html
.. _`scale`: tes3faction/scale.html
.. _`sceneNode`: tes3faction/sceneNode.html
.. _`sceneReference`: tes3faction/sceneReference.html
.. _`skills`: tes3faction/skills.html
.. _`sourceMod`: tes3faction/sourceMod.html
.. _`stolenList`: tes3faction/stolenList.html

Methods
----------------------------------------------------------------------------------------------------

`onInventoryClose`_
    A callback function invoked when an inventory is closed. Typically not used outside of specific purposes.

.. toctree::
    :hidden:

    tes3faction/onInventoryClose

.. _`onInventoryClose`: tes3faction/onInventoryClose.html

.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`boolean`: ../../lua/type/boolean.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`number`: ../../lua/type/number.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
