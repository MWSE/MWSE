applyHealthDamage
====================================================================================================

**Deprecated, please use applyDamage instead.**

Damages the actor.

Returns
----------------------------------------------------------------------------------------------------

`boolean`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

damage (`number`_)
    The amount of damage to apply.

isPlayerAttack (`boolean`_)
    Used to calculate difficulty scaling.

scaleWithDifficulty (`boolean`_)
    Apply difficulty scaling to the damage.

doNotChangeHealth (`boolean`_)
    Do all the audiovisual effects of damage without the health change.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`number`: ../../../lua/type/number.html
