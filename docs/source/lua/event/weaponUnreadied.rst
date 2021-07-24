weaponUnreadied
====================================================================================================

This event is called when a weapon is no longer readied, and pairs with the weaponReadied event. It can be used to reliably tell if a specific weapon is readied for attack. This does not necessarily mean that the animation state has changed for the first time. If an actor equips a weapon while having their hands up to attack with, the event will fire for the new weapon.

Related events: `weaponReadied`_

Event Data
----------------------------------------------------------------------------------------------------

reference
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. The reference associated with the change in readied weapon.

.. _`weaponReadied`: ../../lua/event/weaponReadied.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
