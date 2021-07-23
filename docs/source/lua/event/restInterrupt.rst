restInterrupt
====================================================================================================

This event is invoked when a rest is about to be interrupted. The ambushing spawn can be overridden by changing the creature parameter.

Event Data
----------------------------------------------------------------------------------------------------

resting
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`boolean`_. Read-only. If true, the player is attempting to rest, as opposed to wait.

creature
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

tes3creature|tes3npc|tes3leveledCreature. The creature that is interrupting rest. Can be set to a creature, NPC, or leveled creature.

waiting
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`boolean`_. Read-only. If true, the player is attempting to wait, as opposed to rest.

.. _`boolean`: ../../lua/type/boolean.html
