tes3.adjustSoundVolume
====================================================================================================

Changes the volume of a sound that is playing on a given reference.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

sound (`tes3sound`_, `string`_)
    The sound object, or id of the sound to look for.

reference (`tes3reference`_, `tes3mobileActor`_, `string`_)
    The reference to attach the sound to.

mixChannel (`number`_)
    Default: ``tes3.audioMixType.effects``. The channel to base volume off of. Maps to tes3.audioMixType constants.

volume (`number`_)
    Default: ``1.0``. A value between 0.0 and 1.0 to scale the volume off of.

.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
.. _`tes3sound`: ../../../lua/type/tes3sound.html
