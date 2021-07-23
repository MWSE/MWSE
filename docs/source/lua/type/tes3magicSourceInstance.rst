tes3magicSourceInstance
====================================================================================================

A game structure that keeps track of a magic source on an object.

Properties
----------------------------------------------------------------------------------------------------

`castChanceOverride`_ (`number`_)
    No description available.

`caster`_ (`tes3reference`_)
    Read-only. No description available.

`deleted`_ (`boolean`_)
    The deleted state of the object.

`disabled`_ (`boolean`_)
    The disabled state of the object.

`id`_ (`string`_)
    The unique identifier for the object.

`item`_ (`tes3item`_)
    Read-only. No description available.

`itemData`_ (`tes3itemData`_)
    Read-only. No description available.

`itemID`_ (`string`_)
    Read-only. No description available.

`magicID`_ (`string`_)
    Read-only. No description available.

`modified`_ (`boolean`_)
    The modification state of the object since the last save.

`objectFlags`_ (`number`_)
    The raw flags of the object.

`objectType`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`projectile`_ (`tes3mobileProjectile`_)
    Read-only. No description available.

`source`_ (`tes3alchemy`_, `tes3enchantment`_, `tes3spell`_)
    Read-only. No description available.

`sourceMod`_ (`string`_)
    The filename of the mod that owns this object.

`sourceType`_ (`number`_)
    Read-only. Shows if the source is a spell, enchantment, or alchemy.

`state`_ (`number`_)
    Shows if the state is pre-cast, cast, beginning, working, ending, retired, etc.

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`target`_ (`tes3reference`_)
    Read-only. No description available.

`timestampCastBegin`_ (`number`_)
    No description available.

.. toctree::
    :hidden:

    tes3magicSourceInstance/castChanceOverride
    tes3magicSourceInstance/caster
    tes3magicSourceInstance/deleted
    tes3magicSourceInstance/disabled
    tes3magicSourceInstance/id
    tes3magicSourceInstance/item
    tes3magicSourceInstance/itemData
    tes3magicSourceInstance/itemID
    tes3magicSourceInstance/magicID
    tes3magicSourceInstance/modified
    tes3magicSourceInstance/objectFlags
    tes3magicSourceInstance/objectType
    tes3magicSourceInstance/projectile
    tes3magicSourceInstance/source
    tes3magicSourceInstance/sourceMod
    tes3magicSourceInstance/sourceType
    tes3magicSourceInstance/state
    tes3magicSourceInstance/supportsLuaData
    tes3magicSourceInstance/target
    tes3magicSourceInstance/timestampCastBegin

.. _`castChanceOverride`: tes3magicSourceInstance/castChanceOverride.html
.. _`caster`: tes3magicSourceInstance/caster.html
.. _`deleted`: tes3magicSourceInstance/deleted.html
.. _`disabled`: tes3magicSourceInstance/disabled.html
.. _`id`: tes3magicSourceInstance/id.html
.. _`item`: tes3magicSourceInstance/item.html
.. _`itemData`: tes3magicSourceInstance/itemData.html
.. _`itemID`: tes3magicSourceInstance/itemID.html
.. _`magicID`: tes3magicSourceInstance/magicID.html
.. _`modified`: tes3magicSourceInstance/modified.html
.. _`objectFlags`: tes3magicSourceInstance/objectFlags.html
.. _`objectType`: tes3magicSourceInstance/objectType.html
.. _`projectile`: tes3magicSourceInstance/projectile.html
.. _`source`: tes3magicSourceInstance/source.html
.. _`sourceMod`: tes3magicSourceInstance/sourceMod.html
.. _`sourceType`: tes3magicSourceInstance/sourceType.html
.. _`state`: tes3magicSourceInstance/state.html
.. _`supportsLuaData`: tes3magicSourceInstance/supportsLuaData.html
.. _`target`: tes3magicSourceInstance/target.html
.. _`timestampCastBegin`: tes3magicSourceInstance/timestampCastBegin.html

Methods
----------------------------------------------------------------------------------------------------

`getEffectInstance`_ (`tes3magicEffectInstance`_)
    Gets the magic effect instance for a given effect index and target reference. Effect instances may not all be active on a target, due to reflect, absorption, dispels, different durations and other factors.

`getMagnitudeForIndex`_ (`number`_)
    Gets the magnitude from the casting source for a given effect index.

.. toctree::
    :hidden:

    tes3magicSourceInstance/getEffectInstance
    tes3magicSourceInstance/getMagnitudeForIndex

.. _`getEffectInstance`: tes3magicSourceInstance/getEffectInstance.html
.. _`getMagnitudeForIndex`: tes3magicSourceInstance/getMagnitudeForIndex.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3alchemy`: ../../lua/type/tes3alchemy.html
.. _`tes3enchantment`: ../../lua/type/tes3enchantment.html
.. _`tes3item`: ../../lua/type/tes3item.html
.. _`tes3itemData`: ../../lua/type/tes3itemData.html
.. _`tes3magicEffectInstance`: ../../lua/type/tes3magicEffectInstance.html
.. _`tes3mobileProjectile`: ../../lua/type/tes3mobileProjectile.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
.. _`tes3spell`: ../../lua/type/tes3spell.html
