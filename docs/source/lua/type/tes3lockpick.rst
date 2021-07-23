tes3lockpick
====================================================================================================

A core lockpick object.

Properties
----------------------------------------------------------------------------------------------------

`blocked`_ (`boolean`_)
    The blocked state of the object.

`boundingBox`_ (`tes3boundingBox`_)
    The bounding box for the object.

`condition`_ (`number`_)
    The maximum condition/health of the object.

`deleted`_ (`boolean`_)
    Read-only. The deleted state of the object.

`disabled`_ (`boolean`_)
    Read-only. The disabled state of the object.

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

`quality`_ (`number`_)
    The quality of the object, with how much of a bonus it has.

`scale`_ (`number`_)
    The object's scale.

`sceneNode`_ (`niNode`_)
    The scene graph node for this object.

`sceneReference`_ (`niNode`_)
    The scene graph reference node for this object.

`script`_ (`tes3script`_)
    Read-only. The script that runs on the object.

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

    tes3lockpick/blocked
    tes3lockpick/boundingBox
    tes3lockpick/condition
    tes3lockpick/deleted
    tes3lockpick/disabled
    tes3lockpick/icon
    tes3lockpick/id
    tes3lockpick/mesh
    tes3lockpick/modified
    tes3lockpick/name
    tes3lockpick/nextInCollection
    tes3lockpick/objectFlags
    tes3lockpick/objectType
    tes3lockpick/owningCollection
    tes3lockpick/persistent
    tes3lockpick/previousInCollection
    tes3lockpick/quality
    tes3lockpick/scale
    tes3lockpick/sceneNode
    tes3lockpick/sceneReference
    tes3lockpick/script
    tes3lockpick/sourceMod
    tes3lockpick/sourceless
    tes3lockpick/stolenList
    tes3lockpick/supportsLuaData
    tes3lockpick/value
    tes3lockpick/weight

.. _`blocked`: tes3lockpick/blocked.html
.. _`boundingBox`: tes3lockpick/boundingBox.html
.. _`condition`: tes3lockpick/condition.html
.. _`deleted`: tes3lockpick/deleted.html
.. _`disabled`: tes3lockpick/disabled.html
.. _`icon`: tes3lockpick/icon.html
.. _`id`: tes3lockpick/id.html
.. _`mesh`: tes3lockpick/mesh.html
.. _`modified`: tes3lockpick/modified.html
.. _`name`: tes3lockpick/name.html
.. _`nextInCollection`: tes3lockpick/nextInCollection.html
.. _`objectFlags`: tes3lockpick/objectFlags.html
.. _`objectType`: tes3lockpick/objectType.html
.. _`owningCollection`: tes3lockpick/owningCollection.html
.. _`persistent`: tes3lockpick/persistent.html
.. _`previousInCollection`: tes3lockpick/previousInCollection.html
.. _`quality`: tes3lockpick/quality.html
.. _`scale`: tes3lockpick/scale.html
.. _`sceneNode`: tes3lockpick/sceneNode.html
.. _`sceneReference`: tes3lockpick/sceneReference.html
.. _`script`: tes3lockpick/script.html
.. _`sourceMod`: tes3lockpick/sourceMod.html
.. _`sourceless`: tes3lockpick/sourceless.html
.. _`stolenList`: tes3lockpick/stolenList.html
.. _`supportsLuaData`: tes3lockpick/supportsLuaData.html
.. _`value`: tes3lockpick/value.html
.. _`weight`: tes3lockpick/weight.html

Methods
----------------------------------------------------------------------------------------------------

`__tojson`_ (`string`_)
    Serializes the object to json.

.. toctree::
    :hidden:

    tes3lockpick/__tojson

.. _`__tojson`: tes3lockpick/__tojson.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
