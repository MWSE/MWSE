tes3.loadAnimation
====================================================================================================

Loads an animation and uses it to override existing animations on an actor. Animation groups present in the file will replace the actor's animation groups. The actor's model must be already loaded into memory to have its animations modified. The overridden animations only persist until the actor is unloaded.

Calling this more than once will remove the previous animation override before loading the new animation. Therefore, if applying animations to the player, you should call this before every playAnimation so that different animation mods can co-exist. For NPCs under your mod's control, you only need to do it when the player enters the cell containing the NPC.

Calling this without a file argument will reset the reference's animations to default.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

reference (`tes3mobileActor`_, `tes3reference`_, `string`_)
    The reference to the actor that is having its animations modified.

file (`string`_)
    Optional. The specified animation base file that will be loaded. e.g. For an animation composed of files anim.nif, xanim.nif and xanim.kf, you should pass file="anim.nif". The animation target skeleton _must_ match the skeleton of the actor to work. i.e. 3rd person anims for NPCs and tes3.player, 1st person anims for firstPersonReference.

.. _`string`: ../../../lua/type/string.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
