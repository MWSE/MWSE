tes3alchemy
====================================================================================================

An alchemy game object.

Properties
----------------------------------------------------------------------------------------------------

`autoCalc`_ (`boolean`_)
    If set, the value of the object is automatically calculated.

`blocked`_ (`boolean`_)
    The blocked state of the object.

`boundingBox`_ (`tes3boundingBox`_)
    The bounding box for the object.

`deleted`_ (`boolean`_)
    Read-only. The deleted state of the object.

`disabled`_ (`boolean`_)
    Read-only. The disabled state of the object.

`effects`_ (`table`_)
    Read-only. An array-style table of the tes3effect data on the object.

`flags`_ (`number`_)
    The alchemy item's flags.

`icon`_ (`string`_)
    The path to the object's icon.

`id`_ (`string`_)
    Read-only. The unique identifier for the object.

`mesh`_ (`string`_)
    The path to the object's mesh.

`modified`_ (`boolean`_)
    The modification state of the object since the last save.

`name`_ (`string`_)
    The player-facing name for the object.

`nextInCollection`_ (`tes3object`_)
    The next object in parent collection's list.

`objectFlags`_ (`number`_)
    Read-only. The raw flags of the object.

`objectType`_ (`number`_)
    Read-only. The type of object. Maps to values in tes3.objectType.

`owningCollection`_ (`tes3referenceList`_)
    The collection responsible for holding this object.

`persistent`_ (`boolean`_)
    The persistent flag of the object.

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
    Read-only. The filename of the mod that owns this object.

`sourceless`_ (`boolean`_)
    The soruceless flag of the object.

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

    tes3alchemy/autoCalc
    tes3alchemy/blocked
    tes3alchemy/boundingBox
    tes3alchemy/deleted
    tes3alchemy/disabled
    tes3alchemy/effects
    tes3alchemy/flags
    tes3alchemy/icon
    tes3alchemy/id
    tes3alchemy/mesh
    tes3alchemy/modified
    tes3alchemy/name
    tes3alchemy/nextInCollection
    tes3alchemy/objectFlags
    tes3alchemy/objectType
    tes3alchemy/owningCollection
    tes3alchemy/persistent
    tes3alchemy/previousInCollection
    tes3alchemy/scale
    tes3alchemy/sceneNode
    tes3alchemy/sceneReference
    tes3alchemy/script
    tes3alchemy/sourceMod
    tes3alchemy/sourceless
    tes3alchemy/stolenList
    tes3alchemy/supportsLuaData
    tes3alchemy/value
    tes3alchemy/weight

.. _`autoCalc`: tes3alchemy/autoCalc.html
.. _`blocked`: tes3alchemy/blocked.html
.. _`boundingBox`: tes3alchemy/boundingBox.html
.. _`deleted`: tes3alchemy/deleted.html
.. _`disabled`: tes3alchemy/disabled.html
.. _`effects`: tes3alchemy/effects.html
.. _`flags`: tes3alchemy/flags.html
.. _`icon`: tes3alchemy/icon.html
.. _`id`: tes3alchemy/id.html
.. _`mesh`: tes3alchemy/mesh.html
.. _`modified`: tes3alchemy/modified.html
.. _`name`: tes3alchemy/name.html
.. _`nextInCollection`: tes3alchemy/nextInCollection.html
.. _`objectFlags`: tes3alchemy/objectFlags.html
.. _`objectType`: tes3alchemy/objectType.html
.. _`owningCollection`: tes3alchemy/owningCollection.html
.. _`persistent`: tes3alchemy/persistent.html
.. _`previousInCollection`: tes3alchemy/previousInCollection.html
.. _`scale`: tes3alchemy/scale.html
.. _`sceneNode`: tes3alchemy/sceneNode.html
.. _`sceneReference`: tes3alchemy/sceneReference.html
.. _`script`: tes3alchemy/script.html
.. _`sourceMod`: tes3alchemy/sourceMod.html
.. _`sourceless`: tes3alchemy/sourceless.html
.. _`stolenList`: tes3alchemy/stolenList.html
.. _`supportsLuaData`: tes3alchemy/supportsLuaData.html
.. _`value`: tes3alchemy/value.html
.. _`weight`: tes3alchemy/weight.html

Methods
----------------------------------------------------------------------------------------------------

`__tojson`_ (`string`_)
    Serializes the object to json.

`getActiveEffectCount`_ (`number`_)
    Returns the amount of effects the tes3alchemy object has.

`getFirstIndexOfEffect`_ (`number`_)
    Returns the index of a first effect of a given effectId in the parent tes3alchemy object.

.. toctree::
    :hidden:

    tes3alchemy/__tojson
    tes3alchemy/getActiveEffectCount
    tes3alchemy/getFirstIndexOfEffect

.. _`__tojson`: tes3alchemy/__tojson.html
.. _`getActiveEffectCount`: tes3alchemy/getActiveEffectCount.html
.. _`getFirstIndexOfEffect`: tes3alchemy/getFirstIndexOfEffect.html

Functions
----------------------------------------------------------------------------------------------------

`create`_
    Creates a new alchemy item, which will be stored as part of the current saved game.

.. toctree::
    :hidden:

    tes3alchemy/create

.. _`create`: tes3alchemy/create.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
