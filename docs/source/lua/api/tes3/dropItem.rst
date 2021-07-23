tes3.dropItem
====================================================================================================

Drops one or more items from a reference's inventory onto the ground at their feet. It will unequip the item if it is equipped. The return value will be nil if no matching item was found.

Returns
----------------------------------------------------------------------------------------------------

`tes3reference`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

reference (`tes3mobileActor`_, `tes3reference`_, `string`_)
    The reference whose inventory will be modified.

item (`tes3item`_, `string`_)
    The item to drop.

itemData (`tes3itemData`_)
    Optional. The item data of the specific item to drop. Without this, the first matching item in the inventory will drop.

matchNoItemData (`boolean`_)
    Default: ``false``. If true, matches an item without item data. This can be used when you want to drop an item that isn't equipped (equipped items always have item data).

count (`number`_)
    Default: ``1``. The number of items to drop.

updateGUI (`boolean`_)
    Default: ``true``. If false, the player or contents menu won't be updated.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3item`: ../../../lua/type/tes3item.html
.. _`tes3itemData`: ../../../lua/type/tes3itemData.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
