tes3.claimSpellEffectId
====================================================================================================

This function is used to claim a unique spell effect name and id. This is needed before actually creating a new effect by calling tes3.addMagicEffect. A claimed effect id can be retrieved as: tes3.effect.effectName (just like any regular spell effect).

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

name (`string`_)
    The name of the new spell effect. Must be unique. An error will be thrown if it's non-unique.

id (`number`_)
    A unique number representing the new spell effect. An error will be thrown if it's non-unique.

.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
