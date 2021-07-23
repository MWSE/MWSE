tes3iterator
====================================================================================================

A collection that can be iterated over Contains items in a simple linked list, and stores its head/tail.

Properties
----------------------------------------------------------------------------------------------------

`current`_ (`tes3iteratorNode`_)
    Read-only. A reference for the currently iterated node. This is used by the core game engine, but should not be accessed from lua.

`head`_ (`tes3iteratorNode`_)
    Read-only. The first node in the collection.

`size`_ (`number`_)
    Read-only. The amount of items in the iterator.

`tail`_ (`tes3iteratorNode`_)
    Read-only. The last node in the collection.

.. toctree::
    :hidden:

    tes3iterator/current
    tes3iterator/head
    tes3iterator/size
    tes3iterator/tail

.. _`current`: tes3iterator/current.html
.. _`head`: tes3iterator/head.html
.. _`size`: tes3iterator/size.html
.. _`tail`: tes3iterator/tail.html

Examples
----------------------------------------------------------------------------------------------------

Iterating Lists
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In this example, a list is iterated using the pairs iterator function. The node can be used directly.

.. code-block:: lua

    -- Check player equipment
    for _, stack in pairs(tes3.player.object.equipment) do
        -- stack is type tes3equipmentStack, as tes3.player.object.equipment is a list of tes3equipmentStack.
        mwse.log("Equipment object ID: %s", stack.object.id)
    end

    -- tes3.player.object.spells is a tes3spellList with helper functions.
    -- The actual list is spells.iterator.
    for _, spell in pairs(tes3.player.object.spells.iterator) do
        -- spell is type tes3spell, as tes3.player.object.spells.iterator is a list of spell.
        mwse.log("Spell %s, cost %d", spell.name, spell.magickaCost)
    end


.. _`number`: ../../lua/type/number.html
.. _`tes3iteratorNode`: ../../lua/type/tes3iteratorNode.html
