data
====================================================================================================

`table`_. A generic lua table that data can be written to, and synced to/from the save. All information stored must be valid for serialization to json. For item references, this is the same table as on the tes3itemData structure. To store data that doesn't get serialized to/from the save, use tempData.

.. _`table`: ../../../lua/type/table.html
