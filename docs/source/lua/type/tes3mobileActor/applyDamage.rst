applyDamage
====================================================================================================

Damages the actor, with options to control mitigation and difficulty scaling. Invokes the 'damage' and 'damaged' events, with 'script' source. Returns the actual damage done after armor mitigation and resistance, but before difficulty scaling.

Returns
----------------------------------------------------------------------------------------------------

`number`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

damage (`number`_)
    The amount of damage to apply.

applyArmor (`boolean`_)
    Optional. If armor should mitigate the incoming damage. If the player is the target, armor experience will be gained.

resistAttribute (`number`_)
    Optional. The resistance attribute that is applied to the damage. It can reduce damage or exploit weakness. Uses values from tes3.effectAttributes.

applyDifficulty (`boolean`_)
    Optional. If the game difficulty modifier should be applied. Must be used with the 'playerAttack' argument to apply the correct modifier.

playerAttack (`boolean`_)
    Optional. If the attack came from the player. Used for difficulty calculation.

doNotChangeHealth (`boolean`_)
    Optional. If all armor effects except the health change should be applied. These include hit sounds, armor condition damage, and player experience gain from being hit.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`number`: ../../../lua/type/number.html
