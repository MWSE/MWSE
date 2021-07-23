tes3inventory
====================================================================================================

An inventory composes of an iterator, and flags caching the state of the inventory.

Properties
----------------------------------------------------------------------------------------------------

`flags`_ (`number`_)
    Read-only. Raw bit-based flags.

`iterator`_ (`tes3iterator`_)
    Read-only. Deprecated. Use ``for index, stack in pairs(inventory)`` to iterate over inventory stacks.
	
Direct access to the container that holds the inventory's items.

.. toctree::
    :hidden:

    tes3inventory/flags
    tes3inventory/iterator

.. _`flags`: tes3inventory/flags.html
.. _`iterator`: tes3inventory/iterator.html

Methods
----------------------------------------------------------------------------------------------------

`addItem`_
    Adds an item into the inventory directly. This should not be used, in favor of the tes3.addItem function.

`calculateWeight`_ (`number`_)
    Calculates the weight of all items in the container.

`contains`_ (`boolean`_)
    Checks to see if the inventory contains an item.

`dropItem`_
    Checks to see if the inventory contains an item. This should not be used, instead use the tes3.dropItem function.

`findItemStack`_ (`tes3itemStack`_)
    Searches for an item stack in the inventory.

`removeItem`_
    Removes an item from the inventory directly. This should not be used, in favor of the tes3.removeItem function.

`resolveLeveledItems`_
    Resolves all contained leveled lists and adds the randomized items to the inventory. This should generally not be called directly.

.. toctree::
    :hidden:

    tes3inventory/addItem
    tes3inventory/calculateWeight
    tes3inventory/contains
    tes3inventory/dropItem
    tes3inventory/findItemStack
    tes3inventory/removeItem
    tes3inventory/resolveLeveledItems

.. _`addItem`: tes3inventory/addItem.html
.. _`calculateWeight`: tes3inventory/calculateWeight.html
.. _`contains`: tes3inventory/contains.html
.. _`dropItem`: tes3inventory/dropItem.html
.. _`findItemStack`: tes3inventory/findItemStack.html
.. _`removeItem`: tes3inventory/removeItem.html
.. _`resolveLeveledItems`: tes3inventory/resolveLeveledItems.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`tes3itemStack`: ../../lua/type/tes3itemStack.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
