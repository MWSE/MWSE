equip
====================================================================================================

The equip event fires when an actor is about to equip an item. This event allows scripts to block equipping.

Event Data
----------------------------------------------------------------------------------------------------

item
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3baseObject`_. Read-only. The object being equipped.

reference
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. A reference to the mobile actor that is equipping the item.

itemData
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3itemData`_. Read-only. The item data of item.

.. _`tes3baseObject`: ../../lua/type/tes3baseObject.html
.. _`tes3itemData`: ../../lua/type/tes3itemData.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
