tes3sound
====================================================================================================

A sound object created in the TES3 CS.

Properties
----------------------------------------------------------------------------------------------------

`deleted`_ (`boolean`_)
    The deleted state of the object.

`disabled`_ (`boolean`_)
    The disabled state of the object.

`filename`_ (`string`_)
    Read-only. Path to the sound file.

`id`_ (`string`_)
    Read-only. The unique identifier for the object.

`id`_ (`string`_)
    The unique identifier for the object.

`maxDistance`_ (`number`_)
    The maximum distance the sound will be played at. Defaults to 0. If both this and minDistance are 0, the values are ignored.

`minDistance`_ (`number`_)
    The minimum distance the sound will be played at. Defaults to 0.

`modified`_ (`boolean`_)
    The modification state of the object since the last save.

`objectFlags`_ (`number`_)
    The raw flags of the object.

`objectType`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`sourceMod`_ (`string`_)
    The filename of the mod that owns this object.

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`volume`_ (`number`_)
    Direct access to sound's volume.

.. toctree::
    :hidden:

    tes3sound/deleted
    tes3sound/disabled
    tes3sound/filename
    tes3sound/id
    tes3sound/id
    tes3sound/maxDistance
    tes3sound/minDistance
    tes3sound/modified
    tes3sound/objectFlags
    tes3sound/objectType
    tes3sound/sourceMod
    tes3sound/supportsLuaData
    tes3sound/volume

.. _`deleted`: tes3sound/deleted.html
.. _`disabled`: tes3sound/disabled.html
.. _`filename`: tes3sound/filename.html
.. _`id`: tes3sound/id.html
.. _`id`: tes3sound/id.html
.. _`maxDistance`: tes3sound/maxDistance.html
.. _`minDistance`: tes3sound/minDistance.html
.. _`modified`: tes3sound/modified.html
.. _`objectFlags`: tes3sound/objectFlags.html
.. _`objectType`: tes3sound/objectType.html
.. _`sourceMod`: tes3sound/sourceMod.html
.. _`supportsLuaData`: tes3sound/supportsLuaData.html
.. _`volume`: tes3sound/volume.html

Methods
----------------------------------------------------------------------------------------------------

`__tojson`_ (`string`_)
    Serializes the object to json.

`isLooping`_ (`boolean`_)
    Returns true if the sound is actively looping.

`isPlaying`_ (`boolean`_)
    Returns true if the sound is actively playing.

`play`_
    This metod plays the sound.

`stop`_
    This stops playing the sound.

.. toctree::
    :hidden:

    tes3sound/__tojson
    tes3sound/isLooping
    tes3sound/isPlaying
    tes3sound/play
    tes3sound/stop

.. _`__tojson`: tes3sound/__tojson.html
.. _`isLooping`: tes3sound/isLooping.html
.. _`isPlaying`: tes3sound/isPlaying.html
.. _`play`: tes3sound/play.html
.. _`stop`: tes3sound/stop.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
