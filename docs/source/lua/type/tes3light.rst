tes3light
====================================================================================================

A core light object. This isn't actually a light in the rendering engine, but something like a lamp or torch.

Properties
----------------------------------------------------------------------------------------------------

`boundingBox`_ (`tes3boundingBox`_)
    The bounding box for the object.

`canCarry`_ (`boolean`_)
    Access to the light's flags, determining if the light can be carried.

`color`_ (`table`_)
    Read-only. Access to the light's base colors, in an array-style table of four values. The values can range from 0 to 255.

`deleted`_ (`boolean`_)
    The deleted state of the object.

`disabled`_ (`boolean`_)
    The disabled state of the object.

`flickers`_ (`boolean`_)
    Access to the light's flags, determining if the light attenuation flickers.

`flickersSlowly`_ (`boolean`_)
    Access to the light's flags, determining if the light attenuation flickers slowly.

`icon`_ (`string`_)
    The path to the object's icon.

`id`_ (`string`_)
    The unique identifier for the object.

`isDynamic`_ (`boolean`_)
    Access to the light's flags, determining if the light affects dynamically moving objects.

`isFire`_ (`boolean`_)
    Access to the light's flags, determining if the light represents flame.

`isNegative`_ (`boolean`_)
    Access to the light's flags, determining if the object creates darkness.

`isOffByDefault`_ (`boolean`_)
    Access to the light's flags, determining if the light won't be active initially.

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

`pulses`_ (`boolean`_)
    Access to the light's flags, determining if the light attenuation pulses.

`pulsesSlowly`_ (`boolean`_)
    Access to the light's flags, determining if the light attenuation pulses slowly.

`radius`_ (`number`_)
    The base radius of the light.

`scale`_ (`number`_)
    The object's scale.

`sceneNode`_ (`niNode`_)
    The scene graph node for this object.

`sceneReference`_ (`niNode`_)
    The scene graph reference node for this object.

`script`_ (`tes3script`_)
    Read-only. The script that runs on the object.

`sound`_ (`tes3sound`_)
    Read-only. The sound that runs on the object.

`sourceMod`_ (`string`_)
    The filename of the mod that owns this object.

`stolenList`_ (`tes3iterator`_)
    A list of actors that the object has been stolen from.

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`time`_ (`number`_)
    The amount of time that the light will last.

`value`_ (`number`_)
    The value of the object.

`weight`_ (`number`_)
    The weight of the object.

.. toctree::
    :hidden:

    tes3light/boundingBox
    tes3light/canCarry
    tes3light/color
    tes3light/deleted
    tes3light/disabled
    tes3light/flickers
    tes3light/flickersSlowly
    tes3light/icon
    tes3light/id
    tes3light/isDynamic
    tes3light/isFire
    tes3light/isNegative
    tes3light/isOffByDefault
    tes3light/mesh
    tes3light/modified
    tes3light/name
    tes3light/nextInCollection
    tes3light/objectFlags
    tes3light/objectType
    tes3light/owningCollection
    tes3light/previousInCollection
    tes3light/pulses
    tes3light/pulsesSlowly
    tes3light/radius
    tes3light/scale
    tes3light/sceneNode
    tes3light/sceneReference
    tes3light/script
    tes3light/sound
    tes3light/sourceMod
    tes3light/stolenList
    tes3light/supportsLuaData
    tes3light/time
    tes3light/value
    tes3light/weight

.. _`boundingBox`: tes3light/boundingBox.html
.. _`canCarry`: tes3light/canCarry.html
.. _`color`: tes3light/color.html
.. _`deleted`: tes3light/deleted.html
.. _`disabled`: tes3light/disabled.html
.. _`flickers`: tes3light/flickers.html
.. _`flickersSlowly`: tes3light/flickersSlowly.html
.. _`icon`: tes3light/icon.html
.. _`id`: tes3light/id.html
.. _`isDynamic`: tes3light/isDynamic.html
.. _`isFire`: tes3light/isFire.html
.. _`isNegative`: tes3light/isNegative.html
.. _`isOffByDefault`: tes3light/isOffByDefault.html
.. _`mesh`: tes3light/mesh.html
.. _`modified`: tes3light/modified.html
.. _`name`: tes3light/name.html
.. _`nextInCollection`: tes3light/nextInCollection.html
.. _`objectFlags`: tes3light/objectFlags.html
.. _`objectType`: tes3light/objectType.html
.. _`owningCollection`: tes3light/owningCollection.html
.. _`previousInCollection`: tes3light/previousInCollection.html
.. _`pulses`: tes3light/pulses.html
.. _`pulsesSlowly`: tes3light/pulsesSlowly.html
.. _`radius`: tes3light/radius.html
.. _`scale`: tes3light/scale.html
.. _`sceneNode`: tes3light/sceneNode.html
.. _`sceneReference`: tes3light/sceneReference.html
.. _`script`: tes3light/script.html
.. _`sound`: tes3light/sound.html
.. _`sourceMod`: tes3light/sourceMod.html
.. _`stolenList`: tes3light/stolenList.html
.. _`supportsLuaData`: tes3light/supportsLuaData.html
.. _`time`: tes3light/time.html
.. _`value`: tes3light/value.html
.. _`weight`: tes3light/weight.html

Methods
----------------------------------------------------------------------------------------------------

`getTimeLeft`_ (`number`_)
    Gets the time remaining for a light, given a tes3itemData, tes3reference, or tes3equipmentStack.

.. toctree::
    :hidden:

    tes3light/getTimeLeft

.. _`getTimeLeft`: tes3light/getTimeLeft.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
.. _`tes3sound`: ../../lua/type/tes3sound.html
