tes3spell
====================================================================================================

A spell game object.

Properties
----------------------------------------------------------------------------------------------------

`autoCalc`_ (`boolean`_)
    Determines if the Magicka Cost for the spell is autocalculated.

`castType`_ (`number`_)
    The spell's cast type.

`deleted`_ (`boolean`_)
    The deleted state of the object.

`disabled`_ (`boolean`_)
    The disabled state of the object.

`effects`_ (`table`_)
    An array-style table of the tes3effect data on the object.

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

.. toctree::
    :hidden:

    tes3spell/autoCalc
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
    tes3spell/previousInCollection
    tes3spell/scale
    tes3spell/sceneNode
    tes3spell/sceneReference
    tes3spell/sourceMod

.. _`autoCalc`: tes3spell/autoCalc.html
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
.. _`previousInCollection`: tes3spell/previousInCollection.html
.. _`scale`: tes3spell/scale.html
.. _`sceneNode`: tes3spell/sceneNode.html
.. _`sceneReference`: tes3spell/sceneReference.html
.. _`sourceMod`: tes3spell/sourceMod.html

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

.. toctree::
    :hidden:

    tes3spell/calculateCastChance
    tes3spell/create
    tes3spell/getActiveEffectCount
    tes3spell/getFirstIndexOfEffect

.. _`calculateCastChance`: tes3spell/calculateCastChance.html
.. _`create`: tes3spell/create.html
.. _`getActiveEffectCount`: tes3spell/getActiveEffectCount.html
.. _`getFirstIndexOfEffect`: tes3spell/getFirstIndexOfEffect.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`number`: ../../lua/type/number.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
