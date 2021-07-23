equip
====================================================================================================

Equips an item, optionally adding the item if needed. If the best match is already equipped, it does not perform an unequip-equip cycle, but does return true.

Returns
----------------------------------------------------------------------------------------------------

`boolean`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

item (`tes3item`_, `string`_)
    The item to equip.

itemData (`tes3itemData`_)
    Optional. The item data of the specific item to equip.

addItem (`boolean`_)
    Default: ``false``. If true, the item will be added to the actor's inventory if needed.

selectBestCondition (`boolean`_)
    Default: ``false``. If true, the item in the inventory with the best condition and best charge will be selected.

selectWorstCondition (`boolean`_)
    Default: ``false``. If true, the item in the inventory with the worst condition and worst charge will be selected. Can be useful for selecting tools.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3item`: ../../../lua/type/tes3item.html
.. _`tes3itemData`: ../../../lua/type/tes3itemData.html
