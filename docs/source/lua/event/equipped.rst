equipped
====================================================================================================

The **equipped** event fires after an actor has successfully equipped an item. This event is preceded by the `equip`_ event.

Event Data
----------------------------------------------------------------------------------------------------

actor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3actor`_. Read-only. The actor that has equipped the item.

item
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3baseObject`_. Read-only. The object that has been equipped.

itemData
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3itemData`_. Read-only. The item data of item.

reference
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. actor’s associated reference.

mobile
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3mobileActor`_. Read-only. The mobile actor that has equipped the item.

.. _`equip`: ../../lua/event/equip.html
.. _`tes3actor`: ../../lua/type/tes3actor.html
.. _`tes3baseObject`: ../../lua/type/tes3baseObject.html
.. _`tes3itemData`: ../../lua/type/tes3itemData.html
.. _`tes3mobileActor`: ../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
