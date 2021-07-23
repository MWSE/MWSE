calcSoulValue
====================================================================================================

This event is raised when determining the value of a soul, unmodified by GMSTs. The value can be modified, or used to provide a soul value to NPCs who would normally not be allowed one.

Event Data
----------------------------------------------------------------------------------------------------

value
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`number`_. The value of the actor's soul.

actor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

tes3creature|tes3npc. Read-only. The actor whose soul is being evaluated.

.. _`number`: ../../lua/type/number.html
