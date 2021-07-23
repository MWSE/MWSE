tes3.addItemData
====================================================================================================

Creates an item data if there is room for a new stack in a given inventory. This can be then used to add custom user data or adjust an item's condition. This will return nil if no item data could be allocated for the item -- for example if the reference doesn't have the item in their inventory or each item of that type already has item data.

Returns
----------------------------------------------------------------------------------------------------

`tes3itemData`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

to (`tes3reference`_, `tes3mobileActor`_, `string`_)
    The reference or mobile whose inventory will be modified.

item (`tes3item`_, `string`_)
    The item to create item data for.

updateGUI (`boolean`_)
    Default: ``true``. If false, the player or contents menu won't be updated.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3item`: ../../../lua/type/tes3item.html
.. _`tes3itemData`: ../../../lua/type/tes3itemData.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
