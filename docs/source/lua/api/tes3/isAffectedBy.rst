tes3.isAffectedBy
====================================================================================================

This function check whether a reference is affected by a certain object or effect.

Note reference.object.spells:contains(spellID) will give the same output as this function for abilities, diseases, and curses, because having them in your spell list also makes them affect you.

Returns
----------------------------------------------------------------------------------------------------

`boolean`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

reference (`tes3reference`_, `tes3mobileActor`_, `string`_)
    No description available.

effect (`tes3.effect`_)
    Optional. A tes3.effect constant to check for.

object (`tes3alchemy`_, `tes3enchantment`_, `tes3spell`_, `tes3magicEffect`_, `string`_)
    Optional. An object to check for.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3alchemy`: ../../../lua/type/tes3alchemy.html
.. _`tes3enchantment`: ../../../lua/type/tes3enchantment.html
.. _`tes3magicEffect`: ../../../lua/type/tes3magicEffect.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
.. _`tes3spell`: ../../../lua/type/tes3spell.html
