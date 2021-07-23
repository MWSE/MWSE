spellResist
====================================================================================================

This event is used when calculating a creature's spell resistance, and allows Lua scripts to override the behavior of spell resistance by changing the resistedPercent value. This can be used to enable willpower-based resistance checks, provide specific resistances to specific spells, spells that heal instead of harm, and a variety of new mechanics.

Event Data
----------------------------------------------------------------------------------------------------

sourceInstance
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3magicSourceInstance`_. Read-only. The unique instance of the magic source.

resistedPercent
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`number`_. The percent of the spell that has been resisted. This can be modified, but a value outside the range of 0 to 100 does not have consistent effects. For fire damage, for example, a value over 100 causes the spell to heal the target instead of harming them.

source
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

tes3alchemy|tes3enchantment|tes3spell. Read-only. The magic source.

caster
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. The caster of the spell.

target
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. The target of the spell. For self-targeted spells, this matches caster.

resistAttribute
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`number`_. Read-only. The attribute resisted. This is an index into a tes3mobileActor.effectAttributes. Note that the index here is 0-based, while Lua is 1-based.

effectInstance
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3magicEffectInstance`_. Read-only. The unique instance of the magic effect.

effectIndex
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`number`_. Read-only. The index of the effect in source's effects list.

.. _`number`: ../../lua/type/number.html
.. _`tes3magicEffectInstance`: ../../lua/type/tes3magicEffectInstance.html
.. _`tes3magicSourceInstance`: ../../lua/type/tes3magicSourceInstance.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
