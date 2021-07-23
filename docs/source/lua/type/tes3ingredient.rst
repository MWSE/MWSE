tes3ingredient
====================================================================================================

An ingredient game object.

Properties
----------------------------------------------------------------------------------------------------

`boundingBox`_ (`tes3boundingBox`_)
    The bounding box for the object.

`deleted`_ (`boolean`_)
    The deleted state of the object.

`disabled`_ (`boolean`_)
    The disabled state of the object.

`effectAttributeIds`_ (`table`_)
    Read-only. An array-style table access the attributes associated with the effects.

`effects`_ (`table`_)
    Read-only. An array-style table access to the four ingredient effects. Unlike alchemy or enchanting objects, these are simple numbers representing the effect ID.

`effectsSkillIds`_ (`table`_)
    Read-only. An array-style table access the skills associated with the effects.

`icon`_ (`string`_)
    The path to the object's icon.

`id`_ (`string`_)
    The unique identifier for the object.

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

`value`_ (`number`_)
    The value of the object.

`weight`_ (`number`_)
    The weight of the object.

.. toctree::
    :hidden:

    tes3ingredient/boundingBox
    tes3ingredient/deleted
    tes3ingredient/disabled
    tes3ingredient/effectAttributeIds
    tes3ingredient/effects
    tes3ingredient/effectsSkillIds
    tes3ingredient/icon
    tes3ingredient/id
    tes3ingredient/mesh
    tes3ingredient/modified
    tes3ingredient/name
    tes3ingredient/nextInCollection
    tes3ingredient/objectFlags
    tes3ingredient/objectType
    tes3ingredient/owningCollection
    tes3ingredient/previousInCollection
    tes3ingredient/scale
    tes3ingredient/sceneNode
    tes3ingredient/sceneReference
    tes3ingredient/script
    tes3ingredient/sourceMod
    tes3ingredient/stolenList
    tes3ingredient/supportsLuaData
    tes3ingredient/value
    tes3ingredient/weight

.. _`boundingBox`: tes3ingredient/boundingBox.html
.. _`deleted`: tes3ingredient/deleted.html
.. _`disabled`: tes3ingredient/disabled.html
.. _`effectAttributeIds`: tes3ingredient/effectAttributeIds.html
.. _`effects`: tes3ingredient/effects.html
.. _`effectsSkillIds`: tes3ingredient/effectsSkillIds.html
.. _`icon`: tes3ingredient/icon.html
.. _`id`: tes3ingredient/id.html
.. _`mesh`: tes3ingredient/mesh.html
.. _`modified`: tes3ingredient/modified.html
.. _`name`: tes3ingredient/name.html
.. _`nextInCollection`: tes3ingredient/nextInCollection.html
.. _`objectFlags`: tes3ingredient/objectFlags.html
.. _`objectType`: tes3ingredient/objectType.html
.. _`owningCollection`: tes3ingredient/owningCollection.html
.. _`previousInCollection`: tes3ingredient/previousInCollection.html
.. _`scale`: tes3ingredient/scale.html
.. _`sceneNode`: tes3ingredient/sceneNode.html
.. _`sceneReference`: tes3ingredient/sceneReference.html
.. _`script`: tes3ingredient/script.html
.. _`sourceMod`: tes3ingredient/sourceMod.html
.. _`stolenList`: tes3ingredient/stolenList.html
.. _`supportsLuaData`: tes3ingredient/supportsLuaData.html
.. _`value`: tes3ingredient/value.html
.. _`weight`: tes3ingredient/weight.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
