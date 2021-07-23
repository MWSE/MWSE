tes3.removeItem
====================================================================================================

Removes an item from a given reference's inventory.

Returns
----------------------------------------------------------------------------------------------------

`number`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

reference (`tes3reference`_, `tes3mobileActor`_, `string`_)
    Who to remove items from.

item (`tes3item`_, `string`_)
    The item to remove.

itemData (`tes3itemData`_)
    Optional. The item data for the exact item to remove.

deleteItemData (`tes3itemData`_)
    Optional. Whether to delete the item data after remove succeeds. Automatically set if itemData is used. Does not need to be specified for normal usage.

count (`number`_)
    Default: ``1``. The maximum number of items to remove.

playSound (`boolean`_)
    Default: ``true``. If false, the up/down sound for the item won't be played.

updateGUI (`boolean`_)
    Default: ``true``. If false, the function won't manually resync the player's GUI state. This can result in some optimizations, though `tes3ui.forcePlayerInventoryUpdate()` must manually be called after all inventory updates are finished.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3item`: ../../../lua/type/tes3item.html
.. _`tes3itemData`: ../../../lua/type/tes3itemData.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
