leveledCreaturePicked
====================================================================================================

This event is called after any leveled creature list is resolved. The event may be filtered by its list, and its pick can be overwritten. While this event allows for customization, the leveled list may be altered directly as well.

Event Data
----------------------------------------------------------------------------------------------------

pick
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3creature`_. The result of the pick by vanilla mechanism. This can be modified to change the result to something else.

list
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3leveledCreature`_. Read-only. The list responsible for creating a pick.

.. _`tes3creature`: ../../lua/type/tes3creature.html
.. _`tes3leveledCreature`: ../../lua/type/tes3leveledCreature.html
