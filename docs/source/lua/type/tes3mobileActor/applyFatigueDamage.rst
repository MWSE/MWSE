applyFatigueDamage
====================================================================================================

Damages the actor's fatigue, with accompanying reaction from the reciever. Invokes the 'damageHandToHand' and 'damagedHandToHand' events, with 'script' source. Returns the actual fatigue damage done.

Returns
----------------------------------------------------------------------------------------------------

`number`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

fatigueDamage (`number`_)
    The amount of fatigue damage to apply.

swing (`number`_)
    Optional. The attack swing magnitude, range 0-1. Only modifies hit volume.

alwaysPlayHitVoice (`boolean`_)
    Optional. Always play the hit reaction voiceover.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`number`: ../../../lua/type/number.html
