tes3leveledCreature
====================================================================================================

A leveled creature game object.

Properties
----------------------------------------------------------------------------------------------------

`boundingBox`_ (`tes3boundingBox`_)
    The bounding box for the object.

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

    tes3leveledCreature/boundingBox
    tes3leveledCreature/calculateFromAllLevels
    tes3leveledCreature/chanceForNothing
    tes3leveledCreature/count
    tes3leveledCreature/deleted
    tes3leveledCreature/disabled
    tes3leveledCreature/flags
    tes3leveledCreature/id
    tes3leveledCreature/list
    tes3leveledCreature/modified
    tes3leveledCreature/nextInCollection
    tes3leveledCreature/objectFlags
    tes3leveledCreature/objectType
    tes3leveledCreature/owningCollection
    tes3leveledCreature/previousInCollection
    tes3leveledCreature/scale
    tes3leveledCreature/sceneNode
    tes3leveledCreature/sceneReference
    tes3leveledCreature/sourceMod
    tes3leveledCreature/stolenList
    tes3leveledCreature/supportsLuaData

.. _`boundingBox`: tes3leveledCreature/boundingBox.html
.. _`calculateFromAllLevels`: tes3leveledCreature/calculateFromAllLevels.html
.. _`chanceForNothing`: tes3leveledCreature/chanceForNothing.html
.. _`count`: tes3leveledCreature/count.html
.. _`deleted`: tes3leveledCreature/deleted.html
.. _`disabled`: tes3leveledCreature/disabled.html
.. _`flags`: tes3leveledCreature/flags.html
.. _`id`: tes3leveledCreature/id.html
.. _`list`: tes3leveledCreature/list.html
.. _`modified`: tes3leveledCreature/modified.html
.. _`nextInCollection`: tes3leveledCreature/nextInCollection.html
.. _`objectFlags`: tes3leveledCreature/objectFlags.html
.. _`objectType`: tes3leveledCreature/objectType.html
.. _`owningCollection`: tes3leveledCreature/owningCollection.html
.. _`previousInCollection`: tes3leveledCreature/previousInCollection.html
.. _`scale`: tes3leveledCreature/scale.html
.. _`sceneNode`: tes3leveledCreature/sceneNode.html
.. _`sceneReference`: tes3leveledCreature/sceneReference.html
.. _`sourceMod`: tes3leveledCreature/sourceMod.html
.. _`stolenList`: tes3leveledCreature/stolenList.html
.. _`supportsLuaData`: tes3leveledCreature/supportsLuaData.html

Methods
----------------------------------------------------------------------------------------------------

`pickFrom`_ (`tes3creature`_)
    Chooses a random item from the list, based on the player's level.

.. toctree::
    :hidden:

    tes3leveledCreature/pickFrom

.. _`pickFrom`: tes3leveledCreature/pickFrom.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3creature`: ../../lua/type/tes3creature.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
