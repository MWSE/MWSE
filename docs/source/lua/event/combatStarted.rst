combatStarted
====================================================================================================

The **combatStarted** event occurs after combat has began between two actors. This event is preceded by the `combatStart`_ event.

Event Data
----------------------------------------------------------------------------------------------------

target
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3mobileActor`_. Read-only. The mobile actor who combat has been triggered against.

actor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3mobileActor`_. Read-only. The mobile actor who has entered combat.

.. _`combatStart`: ../../lua/event/combatStart.html
.. _`tes3mobileActor`: ../../lua/type/tes3mobileActor.html
