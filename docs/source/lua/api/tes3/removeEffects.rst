tes3.removeEffects
====================================================================================================

Removes magic effects from a given reference. Requires that either the effect or castType parameter be provided.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

reference (`tes3reference`_)
    Target reference to remove effects from.

effect (`number`_)
    Optional. Maps to tes3.effect constants.

castType (`number`_)
    Optional. Maps to tes3.castType constants.

chance (`number`_)
    Default: ``100``. The chance for the effect to be removed.

removeSpell (`boolean`_)
    Optional. If removing by cast type, determines if the spell should be removed from the target's spell list. Defaults to true if castType is not tes3.castType.spell. This causes diseases and curses to be removed when dispelled.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`number`: ../../../lua/type/number.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
