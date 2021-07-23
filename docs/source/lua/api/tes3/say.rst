tes3.say
====================================================================================================

Plays a sound file, with an optional alteration and subtitle.

**Note**: MP3 voice files must conform to the MPEG Layer-3, 64 Kbps 44100 kHz, 16-bit mono specification.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

reference (`tes3reference`_, `tes3mobileActor`_, `string`_)
    The reference to make say something.

soundPath (`string`_)
    A path to a valid sound file.

pitch (`number`_)
    Default: ``1``. A pitch shift to adjust the sound with.

volume (`number`_)
    Default: ``1``. The volume to play the sound at, relative to the voice mix channel.

forceSubtitle (`boolean`_)
    Default: ``false``. If true a subtitle will be shown, even if subtitles are disabled.

subtitle (`string`_)
    Optional. The subtitle to show if subtitles are enabled, or if forceSubtitle is set.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
