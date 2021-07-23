isGuard
====================================================================================================

The isGuard event triggers whenever the game engine checks if an NPC object is a guard. The guard status can be modified. This alone will not make an NPC behave like a guard, though.

Event Data
----------------------------------------------------------------------------------------------------

npc
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3npc`_. Read-only. The NPC that the guard status is being checked for.

isGuard
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`bool`_. The previously calculated guard status.

.. _`bool`: ../../lua/type/boolean.html
.. _`tes3npc`: ../../lua/type/tes3npc.html
