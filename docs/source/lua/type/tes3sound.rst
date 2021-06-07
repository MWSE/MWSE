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
    Path to the sound file.

`id`_ (`string`_)
    The unique identifier for the object.

`id`_ (`string`_)
    The unique identifier for the object.

`modified`_ (`boolean`_)
    The modification state of the object since the last save.

`objectFlags`_ (`number`_)
    The raw flags of the object.

`objectType`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`sourceMod`_ (`string`_)
    The filename of the mod that owns this object.

`volume`_ (`number`_)
    Direct access to sound's volume.

.. toctree::
    :hidden:

    tes3sound/deleted
    tes3sound/disabled
    tes3sound/filename
    tes3sound/id
    tes3sound/id
    tes3sound/modified
    tes3sound/objectFlags
    tes3sound/objectType
    tes3sound/sourceMod
    tes3sound/volume

.. _`deleted`: tes3sound/deleted.html
.. _`disabled`: tes3sound/disabled.html
.. _`filename`: tes3sound/filename.html
.. _`id`: tes3sound/id.html
.. _`id`: tes3sound/id.html
.. _`modified`: tes3sound/modified.html
.. _`objectFlags`: tes3sound/objectFlags.html
.. _`objectType`: tes3sound/objectType.html
.. _`sourceMod`: tes3sound/sourceMod.html
.. _`volume`: tes3sound/volume.html

Methods
----------------------------------------------------------------------------------------------------

`__tojson`_ (`string`_)
    Serializes the object to json.

`isPlaying`_ (`boolean`_)
    Check if the sound is playing.

`play`_
    This metod plays the sound.

`stop`_
    This stops playing the sound.

.. toctree::
    :hidden:

    tes3sound/__tojson
    tes3sound/isPlaying
    tes3sound/play
    tes3sound/stop

.. _`__tojson`: tes3sound/__tojson.html
.. _`isPlaying`: tes3sound/isPlaying.html
.. _`play`: tes3sound/play.html
.. _`stop`: tes3sound/stop.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`string`: ../../lua/type/string.html
.. _`number`: ../../lua/type/number.html
