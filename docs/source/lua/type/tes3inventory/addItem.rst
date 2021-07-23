addItem
====================================================================================================

Adds an item into the inventory directly. This should not be used, in favor of the tes3.addItem function.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

mobile (`tes3mobileActor`_, `tes3reference`_, `string`_)
    Optional. The mobile actor whose stats will be updated.

item (`tes3item`_)
    The item to add.

itemData (`tes3itemData`_)
    Optional. Any associated item data to add.

count (`number`_)
    Default: ``1``. The number of items to add.

.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3item`: ../../../lua/type/tes3item.html
.. _`tes3itemData`: ../../../lua/type/tes3itemData.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
