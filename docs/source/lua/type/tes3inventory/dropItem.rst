dropItem
====================================================================================================

Checks to see if the inventory contains an item. This should not be used, instead use the tes3.dropItem function.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

mobile (`tes3mobileActor`_, `tes3reference`_, `string`_)
    The mobile actor whose stats will be updated.

item (`tes3item`_, `string`_)
    The item to drop.

itemData (`tes3itemData`_)
    If provided, it will check for the specific data to drop it.

count (`number`_)
    The number of items to drop.

position (`tes3vector3`_)
    A vector determining placement location.

orientation (`tes3vector3`_)
    A vector determining placement rotation.

ignoreItemData (`boolean`_)
    No description available.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3item`: ../../../lua/type/tes3item.html
.. _`tes3itemData`: ../../../lua/type/tes3itemData.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
.. _`tes3vector3`: ../../../lua/type/tes3vector3.html
