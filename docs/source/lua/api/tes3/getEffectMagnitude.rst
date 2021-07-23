tes3.getEffectMagnitude
====================================================================================================

This function returns the total magnitude and total unresisted magnitude of a certain spell effect affecting a reference. It returns a pair of numbers, the first being the post-resistance magnitude (see examples). The unresisted magnitude is the magnitude before the actor's resistances are applied; it is always an integer, so it is used in some UI elements.

Returns
----------------------------------------------------------------------------------------------------

`number, number`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

reference (`tes3reference`_, `tes3mobileActor`_, `string`_)
    An associated mobile should exist for this function to be able to work.

effect (`tes3.effect.* constants`_)
    Effect ID. Can be any of the predefined spell effects, or one added by tes3.claimSpellEffectId.

skill (`tes3.skill.* constants`_)
    Default: ``-1``. If effect parameter specified is: Absorb, Damage, Drain, Fortify or Restore Skill, a skill should be provided. This also applies to any custom spell effect which operates on a certain skill.

attribute (`tes3.attribute.* constants`_)
    Default: ``-1``. If effect parameter specified is: Absorb, Damage, Drain, Fortify or Restore Attribute, an attribute should be provided. This also applies to any custom spell effect which operates on a certain attribute.

Examples
----------------------------------------------------------------------------------------------------

Get both magnitudes.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: lua

    local magnitude, unresistedMagnitude = tes3.getEffectMagnitude{reference = tes3.player, effect = tes3.effect.fireDamage}
    tes3.messageBox(string.format("fortify speed: %f (%f)", magnitude, unresistedMagnitude))


Get magnitude after resistances are applied.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

You can treat the function as if it returns a single value.

.. code-block:: lua

    local magnitude = tes3.getEffectMagnitude{reference = tes3.player, effect = tes3.effect.fortifyAttribute, attribute = tes3.attribute.speed}
    tes3.messageBox(string.format("fortify speed: %f", magnitude))


.. _`string`: ../../../lua/type/string.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
