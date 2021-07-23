lockPick
====================================================================================================

This event fires when a lock is being picked.

Event Data
----------------------------------------------------------------------------------------------------

picker
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

tes3mobileNpc. Read-only. The Mobile NPC doing the disarming.

lockPresent
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`bool`_. Read-only. Indicates if a lock is present on the reference.

lockData
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

tes3lockAttachmentNode. Read-only. The lock data of the reference.

chance
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`number`_. Read-only. The chance the lockpick will be successful.

tool
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3item`_. Read-only. The item the picker is using to pick the lock.

reference
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. The reference that triggered the event (container, door, etc.).

toolItemData
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3itemData`_. Read-only. The item data for the tool.

.. _`bool`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`tes3item`: ../../lua/type/tes3item.html
.. _`tes3itemData`: ../../lua/type/tes3itemData.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
