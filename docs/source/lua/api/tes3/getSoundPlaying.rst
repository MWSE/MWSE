tes3.getSoundPlaying
====================================================================================================

Without a reference, this function returns true if the sound is playing unattached or on any reference. With a reference, it returns true if the sound is playing on that specific reference.

Returns
----------------------------------------------------------------------------------------------------

`boolean`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

sound (`tes3sound`_, `string`_)
    The sound object, or the ID of the sound to look for.

reference (`tes3reference`_, `tes3mobileActor`_, `string`_)
    Optional. No description available.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
.. _`tes3sound`: ../../../lua/type/tes3sound.html
