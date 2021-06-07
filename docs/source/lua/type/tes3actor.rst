tes3actor
====================================================================================================

An Actor (not to be confused with a Mobile Actor) is an object that can be cloned and that has an inventory. Creatures, NPCs, and containers are all considered actors.

It is standard for creatures and NPCs to be composed of an actor and a mobile actor, linked together with a reference.

Properties
----------------------------------------------------------------------------------------------------

`actorFlags`_ (`number`_)
    A number representing the actor flags. Truly a bit field.

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

`sourceMod`_ (`string`_)
    The filename of the mod that owns this object.

`stolenList`_ (`tes3iterator`_)
    A list of actors that the object has been stolen from.

.. toctree::
    :hidden:

    tes3actor/actorFlags
    tes3actor/boundingBox
    tes3actor/cloneCount
    tes3actor/deleted
    tes3actor/disabled
    tes3actor/equipment
    tes3actor/id
    tes3actor/inventory
    tes3actor/modified
    tes3actor/nextInCollection
    tes3actor/objectFlags
    tes3actor/objectType
    tes3actor/owningCollection
    tes3actor/previousInCollection
    tes3actor/scale
    tes3actor/sceneNode
    tes3actor/sceneReference
    tes3actor/sourceMod
    tes3actor/stolenList

.. _`actorFlags`: tes3actor/actorFlags.html
.. _`boundingBox`: tes3actor/boundingBox.html
.. _`cloneCount`: tes3actor/cloneCount.html
.. _`deleted`: tes3actor/deleted.html
.. _`disabled`: tes3actor/disabled.html
.. _`equipment`: tes3actor/equipment.html
.. _`id`: tes3actor/id.html
.. _`inventory`: tes3actor/inventory.html
.. _`modified`: tes3actor/modified.html
.. _`nextInCollection`: tes3actor/nextInCollection.html
.. _`objectFlags`: tes3actor/objectFlags.html
.. _`objectType`: tes3actor/objectType.html
.. _`owningCollection`: tes3actor/owningCollection.html
.. _`previousInCollection`: tes3actor/previousInCollection.html
.. _`scale`: tes3actor/scale.html
.. _`sceneNode`: tes3actor/sceneNode.html
.. _`sceneReference`: tes3actor/sceneReference.html
.. _`sourceMod`: tes3actor/sourceMod.html
.. _`stolenList`: tes3actor/stolenList.html

Methods
----------------------------------------------------------------------------------------------------

`onInventoryClose`_
    A callback function invoked when an inventory is closed. Typically not used outside of specific purposes.

.. toctree::
    :hidden:

    tes3actor/onInventoryClose

.. _`onInventoryClose`: tes3actor/onInventoryClose.html

.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`boolean`: ../../lua/type/boolean.html
.. _`string`: ../../lua/type/string.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`number`: ../../lua/type/number.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
