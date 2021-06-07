tes3iterator
====================================================================================================

A collection that can be iterated over Contains items in a simple linked list, and stores its head/tail.

Properties
----------------------------------------------------------------------------------------------------

`current`_ (`tes3iteratorNode`_)
    A reference for the currently iterated node. This is used by the core game engine, but should not be accessed from lua.

`head`_ (`tes3iteratorNode`_)
    The first node in the collection.

`size`_ (`number`_)
    The amount of items in the iterator.

`tail`_ (`tes3iteratorNode`_)
    The last node in the collection.

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

    local tes3iterator = tes3.player.object.equipment
    for _, tes3iteratorNode in pairs(tes3iterator) do
        -- tes3iteratorNode is type tes3equipmentStack, as tes3.player.object.equipment is a list of tes3equipmentStack.
        mwse.log("Object ID: %s", tes3iteratorNode.object.id)
    end


.. _`tes3iteratorNode`: ../../lua/type/tes3iteratorNode.html
.. _`number`: ../../lua/type/number.html
