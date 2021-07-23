tes3.streamMusic
====================================================================================================

This function interrupts the current music to play the specified music track.

Returns
----------------------------------------------------------------------------------------------------

`boolean`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

path (`string`_)
    Path to the music file, relative to Data Files/music/.

situation (`number`_)
    Default: ``tes3.musicSituation.uninterruptible``. Determines what kind of gameplay situation the music should stay active for. Explore music plays during non-combat, and ends when combat starts. Combat music starts during combat, and ends when combat ends. Uninterruptible music always plays, ending only when the track does.

crossfade (`number`_)
    Default: ``1.0``. The duration in seconds of the crossfade from the old to the new track. The default is 1.0.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
