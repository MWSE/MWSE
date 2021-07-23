tes3.cast
====================================================================================================

Casts a spell from a given reference to a target reference. Touch effects will hit the target at any range, while target effects will create a projectile. By default, the spell always casts successfully and does not consume magicka. By default, an actor casting will stop and perform its cast animation, but the 'instant' flag can start the cast instantly, and allow more control over the spell.

When the caster is the player, the target parameter is optional; without a target, the player's touch effects will only hit targets in front of them, and target effects will create a projectile in the direction the player is facing. Currently as a limitation, instant must be true to allow the player to cast spells.

Returns
----------------------------------------------------------------------------------------------------

`boolean`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

reference (`tes3reference`_, `tes3mobileActor`_, `string`_)
    The caster reference.

target (`tes3reference`_, `tes3mobileActor`_, `string`_)
    The target reference. Optional only if the caster is the player.

spell (`tes3spell`_, `string`_)
    The spell the caster uses.

instant (`boolean`_)
    Default: ``false``. When true, the spell is cast instantly. No animation is performed.

alwaysSucceeds (`boolean`_)
    Default: ``true``. When true, the spell cannot fail and does not consume magicka. When false, it is cast using the actor's spell skill, and requires and takes enough magicka to cast. Only applies when 'instant' is true.

bypassResistances (`boolean`_)
    Default: ``false``. The spell will bypass the target's resistances. Only applies when 'instant' is true.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
.. _`tes3spell`: ../../../lua/type/tes3spell.html
