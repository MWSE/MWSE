tes3.addMagicEffect
====================================================================================================

This function creates a new custom magic effect. The effect can be scripted through lua.

Returns
----------------------------------------------------------------------------------------------------

`tes3effect`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

id (`tes3.effect`_)
    Id of the new effect. Maps to tes3.effect.* added with tes3.claimSpellEffectId. If the effect of this id already exists, an error will be thrown.

name (`string`_)
    Default: ``Unnamed Effect``. Name of the effect.

baseCost (`number`_)
    Default: ``1``. Base magicka cost for the effect.

school (`tes3.magicSchool`_)
    Default: ``tes3.magicSchool.alteration``. The magic school the new effect will be assigned to. Maps to tes3.magicSchool.* constants

size (`number`_)
    Default: ``1``. No description available.

sizeCap (`number`_)
    Default: ``1``. No description available.

speed (`number`_)
    Default: ``1``. No description available.

description (`string`_)
    Default: ``No description available.``. Description for the effect.

lighting (`table`_)
    Optional. No description available.

icon (`string`_)
    Optional. Path to the effect icon. Must be a string no longer than 31 characters long.

particleTexture (`string`_)
    Optional. Path to the particle texture to use for the effect. Must be a string no longer than 31 characters long.

castSound (`string`_)
    Optional. Path to the sound which will be played on casting a spell with this effect. Must be a string no longer than 31 characters long.

boltSound (`string`_)
    Optional. Path to the sound which will be played when a spell with this effect is in flight. Must be a string no longer than 31 characters long.

hitSound (`string`_)
    Optional. Path to the sound which will be played when a spell with this effect hits something. Must be a string no longer than 31 characters long.

areaSound (`string`_)
    Optional. Must be a string no longer than 31 characters long.

castVFX (`tes3physicalObject`_)
    Optional. No description available.

boltVFX (`tes3physicalObject`_)
    Optional. No description available.

hitVFX (`tes3physicalObject`_)
    Optional. No description available.

areaVFX (`tes3physicalObject`_)
    Optional. No description available.

allowEnchanting (`boolean`_)
    Default: ``true``. Can this effect be used in a custom enchantment?

allowSpellmaking (`boolean`_)
    Default: ``true``. Can this effect be used in a custom spell?

appliesOnce (`boolean`_)
    Default: ``true``. No description available.

canCastSelf (`boolean`_)
    Default: ``true``. Can this effect used with cast on self range?

canCastTarget (`boolean`_)
    Default: ``true``. Can this effect be used with cast on target range?

canCastTouch (`boolean`_)
    Default: ``true``. Can this effect be used with cast on touch range?

casterLinked (`boolean`_)
    Default: ``true``. No description available.

hasContinuousVFX (`boolean`_)
    Default: ``true``. Will the effect's VFX be played during its whole duration?

hasNoDuration (`boolean`_)
    Default: ``true``. No description available.

hasNoMagnitude (`boolean`_)
    Default: ``true``. No description available.

illegalDaedra (`boolean`_)
    Default: ``true``. Is this effect illegal to use in public, because it summons Daedra? Note: this mechanic is not implemented in the game. Some mods might rely on this parameter.

isHarmful (`boolean`_)
    Default: ``true``. Is this effect considered harmful and casting it can be considered as an attack?

nonRecastable (`boolean`_)
    Default: ``true``. Can this effect be recast while it already is in duration?

targetsAttributes (`boolean`_)
    Default: ``true``. No description available.

targetsSkills (`boolean`_)
    Default: ``true``. No description available.

unreflectable (`boolean`_)
    Default: ``true``. Can the effect be reflected?

usesNegativeLighting (`boolean`_)
    Default: ``true``. No description available.

onTick (`function`_)
    Optional. A function which will be called on each tick of a spell containing this effect.

onCollision (`function`_)
    Optional. A function which will be called when a spell containing this spell effect collides with something.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`function`: ../../../lua/type/function.html
.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`table`: ../../../lua/type/table.html
.. _`tes3effect`: ../../../lua/type/tes3effect.html
.. _`tes3physicalObject`: ../../../lua/type/tes3physicalObject.html
