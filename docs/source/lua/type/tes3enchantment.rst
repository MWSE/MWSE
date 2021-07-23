tes3enchantment
====================================================================================================

An enchantment game object.

Properties
----------------------------------------------------------------------------------------------------

`castType`_ (`number`_)
    The enchantment's cast type.

`chargeCost`_ (`number`_)
    The cost of using the enchantment.

`deleted`_ (`boolean`_)
    The deleted state of the object.

`disabled`_ (`boolean`_)
    The disabled state of the object.

`effects`_ (`table`_)
    Read-only. An array-style table of the tes3effect data on the object.

`flags`_ (`number`_)
    A bit field for the enchantment's flags.

`id`_ (`string`_)
    The unique identifier for the object.

`magickaCost`_ (`number`_)
    No description available.

`maxCharge`_ (`number`_)
    The maximum charge for the associated enchantment.

`modified`_ (`boolean`_)
    The modification state of the object since the last save.

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

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

.. toctree::
    :hidden:

    tes3enchantment/castType
    tes3enchantment/chargeCost
    tes3enchantment/deleted
    tes3enchantment/disabled
    tes3enchantment/effects
    tes3enchantment/flags
    tes3enchantment/id
    tes3enchantment/magickaCost
    tes3enchantment/maxCharge
    tes3enchantment/modified
    tes3enchantment/nextInCollection
    tes3enchantment/objectFlags
    tes3enchantment/objectType
    tes3enchantment/owningCollection
    tes3enchantment/previousInCollection
    tes3enchantment/scale
    tes3enchantment/sceneNode
    tes3enchantment/sceneReference
    tes3enchantment/sourceMod
    tes3enchantment/supportsLuaData

.. _`castType`: tes3enchantment/castType.html
.. _`chargeCost`: tes3enchantment/chargeCost.html
.. _`deleted`: tes3enchantment/deleted.html
.. _`disabled`: tes3enchantment/disabled.html
.. _`effects`: tes3enchantment/effects.html
.. _`flags`: tes3enchantment/flags.html
.. _`id`: tes3enchantment/id.html
.. _`magickaCost`: tes3enchantment/magickaCost.html
.. _`maxCharge`: tes3enchantment/maxCharge.html
.. _`modified`: tes3enchantment/modified.html
.. _`nextInCollection`: tes3enchantment/nextInCollection.html
.. _`objectFlags`: tes3enchantment/objectFlags.html
.. _`objectType`: tes3enchantment/objectType.html
.. _`owningCollection`: tes3enchantment/owningCollection.html
.. _`previousInCollection`: tes3enchantment/previousInCollection.html
.. _`scale`: tes3enchantment/scale.html
.. _`sceneNode`: tes3enchantment/sceneNode.html
.. _`sceneReference`: tes3enchantment/sceneReference.html
.. _`sourceMod`: tes3enchantment/sourceMod.html
.. _`supportsLuaData`: tes3enchantment/supportsLuaData.html

Functions
----------------------------------------------------------------------------------------------------

`create`_
    Creates a new enchantment object, which will be stored as part of the current saved game.

.. toctree::
    :hidden:

    tes3enchantment/create

.. _`create`: tes3enchantment/create.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
