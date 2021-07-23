tes3spell
====================================================================================================

A spell game object.

Properties
----------------------------------------------------------------------------------------------------

`alwaysSucceeds`_ (`boolean`_)
    A flag that determines if casting the spell will always succeed.

`autoCalc`_ (`boolean`_)
    Determines if the magicka cost for the spell is autocalculated, and if the spell may be automatically assigned to NPCs if they are skillful enough to cast it.

`basePurchaseCost`_ (`number`_)
    Calculates the base cost to purchase the spell. This value is not adjusted by mercantile skills.

`castType`_ (`number`_)
    The spell's cast type.

`deleted`_ (`boolean`_)
    The deleted state of the object.

`disabled`_ (`boolean`_)
    The disabled state of the object.

`effects`_ (`table`_)
    Read-only. An array-style table of the tes3effect data on the object.

`flags`_ (`number`_)
    A bit field for the spell's flags.

`id`_ (`string`_)
    The unique identifier for the object.

`magickaCost`_ (`number`_)
    No description available.

`modified`_ (`boolean`_)
    The modification state of the object since the last save.

`name`_ (`string`_)
    The name of the spell.

`nextInCollection`_ (`tes3object`_)
    The next object in parent collection's list.

`objectFlags`_ (`number`_)
    The raw flags of the object.

`objectType`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`owningCollection`_ (`tes3referenceList`_)
    The collection responsible for holding this object.

`playerStart`_ (`boolean`_)
    A flag that determines if the spell may be assigned to the player at character generation if the player has enough skill to cast it.

`previousInCollection`_ (`tes3object`_)
    The previous object in parent collection's list.

`scale`_ (`number`_)
    The object's scale.

`sceneNode`_ (`niNode`_)
    The scene graph node for this object.

`sceneReference`_ (`niNode`_)
    The scene graph reference node for this object.

`sourceMod`_ (`string`_)
    The filename of the mod that owns this object.

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

.. toctree::
    :hidden:

    tes3spell/alwaysSucceeds
    tes3spell/autoCalc
    tes3spell/basePurchaseCost
    tes3spell/castType
    tes3spell/deleted
    tes3spell/disabled
    tes3spell/effects
    tes3spell/flags
    tes3spell/id
    tes3spell/magickaCost
    tes3spell/modified
    tes3spell/name
    tes3spell/nextInCollection
    tes3spell/objectFlags
    tes3spell/objectType
    tes3spell/owningCollection
    tes3spell/playerStart
    tes3spell/previousInCollection
    tes3spell/scale
    tes3spell/sceneNode
    tes3spell/sceneReference
    tes3spell/sourceMod
    tes3spell/supportsLuaData

.. _`alwaysSucceeds`: tes3spell/alwaysSucceeds.html
.. _`autoCalc`: tes3spell/autoCalc.html
.. _`basePurchaseCost`: tes3spell/basePurchaseCost.html
.. _`castType`: tes3spell/castType.html
.. _`deleted`: tes3spell/deleted.html
.. _`disabled`: tes3spell/disabled.html
.. _`effects`: tes3spell/effects.html
.. _`flags`: tes3spell/flags.html
.. _`id`: tes3spell/id.html
.. _`magickaCost`: tes3spell/magickaCost.html
.. _`modified`: tes3spell/modified.html
.. _`name`: tes3spell/name.html
.. _`nextInCollection`: tes3spell/nextInCollection.html
.. _`objectFlags`: tes3spell/objectFlags.html
.. _`objectType`: tes3spell/objectType.html
.. _`owningCollection`: tes3spell/owningCollection.html
.. _`playerStart`: tes3spell/playerStart.html
.. _`previousInCollection`: tes3spell/previousInCollection.html
.. _`scale`: tes3spell/scale.html
.. _`sceneNode`: tes3spell/sceneNode.html
.. _`sceneReference`: tes3spell/sceneReference.html
.. _`sourceMod`: tes3spell/sourceMod.html
.. _`supportsLuaData`: tes3spell/supportsLuaData.html

Functions
----------------------------------------------------------------------------------------------------

`calculateCastChance`_ (`number`_)
    Calculates the chance that a caster can cast a given spell.

`create`_
    Creates a new spell object, which will be stored as part of the current saved game.

`getActiveEffectCount`_ (`number`_)
    Gets the number of active effects in the spell effect table.

`getFirstIndexOfEffect`_ (`number`_)
    Gets the first index of an effect ID in the spell effect table.

`getLeastProficientEffect`_
    Returns the effect of the spell that a given actor is least proficient with.

`getLeastProficientSchool`_
    Returns the school of the least proficient effect on the spell, for a given actor.

.. toctree::
    :hidden:

    tes3spell/calculateCastChance
    tes3spell/create
    tes3spell/getActiveEffectCount
    tes3spell/getFirstIndexOfEffect
    tes3spell/getLeastProficientEffect
    tes3spell/getLeastProficientSchool

.. _`calculateCastChance`: tes3spell/calculateCastChance.html
.. _`create`: tes3spell/create.html
.. _`getActiveEffectCount`: tes3spell/getActiveEffectCount.html
.. _`getFirstIndexOfEffect`: tes3spell/getFirstIndexOfEffect.html
.. _`getLeastProficientEffect`: tes3spell/getLeastProficientEffect.html
.. _`getLeastProficientSchool`: tes3spell/getLeastProficientSchool.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
