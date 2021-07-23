musicSelectTrack
====================================================================================================

The musicSelectTrack event occurs when new music is needed after a playing music track ends, or the combat situation changes. It allows you to select your own music for the current conditions by setting eventData.music. However, it does not control transitions to combat music, which in the future will be available in another event.

Event Data
----------------------------------------------------------------------------------------------------

music
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`string`_. If set to the path of a given track (relative to Data Files/music), it will play the given path instead of a random one.

situation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`number`_. Read-only. Maps to tes3.musicSituation.*, indicating combat or non-combat music.

.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
