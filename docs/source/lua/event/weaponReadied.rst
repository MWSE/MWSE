weaponReadied
====================================================================================================

This event is called when a weapon is readied, and pairs with the weaponUnreadied event. It can be used to reliably tell if a specific weapon is readied for attack. This does not necessarily mean that the animation state has changed for the first time.

Event Data
----------------------------------------------------------------------------------------------------

reference
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. The reference associated with the change in readied weapon.

weaponStack
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3baseObject`_. Read-only. The bundle that contains the newly readied weapon and its associated data.

.. _`tes3baseObject`: ../../lua/type/tes3baseObject.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
