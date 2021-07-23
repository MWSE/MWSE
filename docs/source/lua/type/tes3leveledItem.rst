tes3leveledItem
====================================================================================================

A leveled creature game object.

Properties
----------------------------------------------------------------------------------------------------

`boundingBox`_ (`tes3boundingBox`_)
    The bounding box for the object.

`calculateForEachItem`_ (`boolean`_)
    If true, each entry is rolled in the leveled list when resolving items. This can result in multiple items from one list being added.

`calculateFromAllLevels`_ (`boolean`_)
    If true, the leveled list is picked from the entire level range, instead of just the highest.

`chanceForNothing`_ (`number`_)
    Read-only. The percent chance, from 0 to 100, for no object to be chosen.

`count`_ (`number`_)
    Read-only. The number of possible options in the leveled object container.

`deleted`_ (`boolean`_)
    The deleted state of the object.

`disabled`_ (`boolean`_)
    The disabled state of the object.

`flags`_ (`number`_)
    Read-only. A numerical representation of bit flags for the object.

`id`_ (`string`_)
    The unique identifier for the object.

`list`_ (`tes3iterator`_)
    Read-only. The collection that itself, containing tes3leveledListNodes.

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

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

.. toctree::
    :hidden:

    tes3leveledItem/boundingBox
    tes3leveledItem/calculateForEachItem
    tes3leveledItem/calculateFromAllLevels
    tes3leveledItem/chanceForNothing
    tes3leveledItem/count
    tes3leveledItem/deleted
    tes3leveledItem/disabled
    tes3leveledItem/flags
    tes3leveledItem/id
    tes3leveledItem/list
    tes3leveledItem/modified
    tes3leveledItem/nextInCollection
    tes3leveledItem/objectFlags
    tes3leveledItem/objectType
    tes3leveledItem/owningCollection
    tes3leveledItem/previousInCollection
    tes3leveledItem/scale
    tes3leveledItem/sceneNode
    tes3leveledItem/sceneReference
    tes3leveledItem/sourceMod
    tes3leveledItem/stolenList
    tes3leveledItem/supportsLuaData

.. _`boundingBox`: tes3leveledItem/boundingBox.html
.. _`calculateForEachItem`: tes3leveledItem/calculateForEachItem.html
.. _`calculateFromAllLevels`: tes3leveledItem/calculateFromAllLevels.html
.. _`chanceForNothing`: tes3leveledItem/chanceForNothing.html
.. _`count`: tes3leveledItem/count.html
.. _`deleted`: tes3leveledItem/deleted.html
.. _`disabled`: tes3leveledItem/disabled.html
.. _`flags`: tes3leveledItem/flags.html
.. _`id`: tes3leveledItem/id.html
.. _`list`: tes3leveledItem/list.html
.. _`modified`: tes3leveledItem/modified.html
.. _`nextInCollection`: tes3leveledItem/nextInCollection.html
.. _`objectFlags`: tes3leveledItem/objectFlags.html
.. _`objectType`: tes3leveledItem/objectType.html
.. _`owningCollection`: tes3leveledItem/owningCollection.html
.. _`previousInCollection`: tes3leveledItem/previousInCollection.html
.. _`scale`: tes3leveledItem/scale.html
.. _`sceneNode`: tes3leveledItem/sceneNode.html
.. _`sceneReference`: tes3leveledItem/sceneReference.html
.. _`sourceMod`: tes3leveledItem/sourceMod.html
.. _`stolenList`: tes3leveledItem/stolenList.html
.. _`supportsLuaData`: tes3leveledItem/supportsLuaData.html

Methods
----------------------------------------------------------------------------------------------------

`pickFrom`_ (`tes3item`_)
    Chooses a random item from the list, based on the player's level.

.. toctree::
    :hidden:

    tes3leveledItem/pickFrom

.. _`pickFrom`: tes3leveledItem/pickFrom.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3item`: ../../lua/type/tes3item.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
