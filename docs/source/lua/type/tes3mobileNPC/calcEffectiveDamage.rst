calcEffectiveDamage
====================================================================================================

Calculates the damage that would be inflicted to an actor after armor and/or resistance. Returns the actual damage done after armor mitigation and resistance, but before difficulty scaling.

Returns
----------------------------------------------------------------------------------------------------

`number`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

damage (`number`_)
    The amount of damage to apply.

applyArmor (`boolean`_)
    Optional. If armor should mitigate the incoming damage.

resistAttribute (`number`_)
    Optional. The resistance attribute that is applied to the damage. It can reduce damage or exploit weakness. Uses values from tes3.effectAttributes.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`number`: ../../../lua/type/number.html
