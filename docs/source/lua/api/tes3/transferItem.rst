tes3.transferItem
====================================================================================================

Moves one or more items from one reference to another. Returns the actual amount of items successfully transferred.

Returns
----------------------------------------------------------------------------------------------------

`number`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

from (`tes3reference`_, `tes3mobileActor`_, `string`_)
    Who to take items from.

to (`tes3reference`_, `tes3mobileActor`_, `string`_)
    Who to give items to.

item (`tes3item`_, `string`_)
    The item to transfer.

itemData (`tes3itemData`_)
    Optional. The specific item data to transfer if, for example, you want to transfer a specific player item.

count (`number`_)
    Default: ``1``. The maximum number of items to transfer.

playSound (`boolean`_)
    Default: ``true``. If false, the up/down sound for the item won't be played.

limitCapacity (`boolean`_)
    Default: ``true``. If false, items can be placed into containers that shouldn't normally be allowed. This includes organic containers, and containers that are full.

updateGUI (`boolean`_)
    Default: ``true``. If false, the function won't manually resync the player's GUI state. This can result in some optimizations, though `tes3ui.forcePlayerInventoryUpdate()` must manually be called after all inventory updates are finished.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3item`: ../../../lua/type/tes3item.html
.. _`tes3itemData`: ../../../lua/type/tes3itemData.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
