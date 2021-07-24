equip
====================================================================================================

The equip event fires when an actor is about to equip an item, i.e. just before the item is equipped. This event allows scripts to block equipping. See `equipped`_ for the post-equip event.

The item must not be removed from the inventory during this event, as the item and item data is still being referenced by the initiator of the equip action. If you want to consume the item, block the equip and remove it at a later time. Removing the item during this event can cause inventory desync bugs.

Related events: `equipped`_, `unequipped`_

Event Data
----------------------------------------------------------------------------------------------------

item
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3baseObject`_. Read-only. The object being equipped.

reference
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. The reference to the mobile actor that is equipping the item.

itemData
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3itemData`_. Read-only. The item data of item.

.. _`equipped`: ../../lua/event/equipped.html
.. _`unequipped`: ../../lua/event/unequipped.html
.. _`tes3baseObject`: ../../lua/type/tes3baseObject.html
.. _`tes3itemData`: ../../lua/type/tes3itemData.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
