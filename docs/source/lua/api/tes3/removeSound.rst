tes3.removeSound
====================================================================================================

Stops a sound playing. Without a reference, it will match unattached sounds. With a reference, it will only match a sound playing on that specific reference.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

sound (`tes3sound`_, `string`_)
    The sound object, or id of the sound to look for.

reference (`tes3reference`_, `tes3mobileActor`_, `string`_)
    Optional. The reference the sound is attached to.

.. _`string`: ../../../lua/type/string.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
.. _`tes3sound`: ../../../lua/type/tes3sound.html
