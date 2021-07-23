tes3.playSound
====================================================================================================

Plays a sound on a given reference. Provides control over volume (including volume channel), pitch, and loop control.

Returns
----------------------------------------------------------------------------------------------------

`boolean`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

sound (`tes3sound`_, `string`_)
    The sound object, or id of the sound to look for.

reference (`tes3reference`_, `tes3mobileActor`_, `string`_)
    Optional. The reference to attach the sound to.

loop (`boolean`_)
    Default: ``false``. If true, the sound will loop.

mixChannel (`number`_)
    Default: ``tes3.audioMixType.effects``. The channel to base volume off of. Maps to tes3.audioMixType constants.

volume (`number`_)
    Default: ``1.0``. A value between 0.0 and 1.0 to scale the volume off of.

pitch (`number`_)
    Default: ``1.0``. The pitch-shift multiplier. For 22kHz audio (most typical) it can have the range [0.005, 4.5]; for 44kHz audio it can have the range [0.0025, 2.25].

soundPath (`string`_)
    Optional. The path to a custom soundfile (useful for playing sounds that are not registered in the Construction Set). Starts in Data Files\Sound.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
.. _`tes3sound`: ../../../lua/type/tes3sound.html
