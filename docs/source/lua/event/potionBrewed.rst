potionBrewed
====================================================================================================

The potionBrewed event is raised when the player brews a new potion.

Event Data
----------------------------------------------------------------------------------------------------

calcinator
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3apparatus`_. Read-only. The calcinator apparatus used to create the potion.

object
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3alchemy`_. Read-only. The potion created by the player.

retort
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3apparatus`_. Read-only. The retort apparatus used to create the potion.

ingredients
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`table`_. Read-only. An array of up to 4 tes3ingredient objects, which were used to create the potion.

alembic
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3apparatus`_. Read-only. The alembic apparatus used to create the potion.

mortar
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3apparatus`_. Read-only. The mortar apparatus used to create the potion.

.. _`table`: ../../lua/type/table.html
.. _`tes3alchemy`: ../../lua/type/tes3alchemy.html
.. _`tes3apparatus`: ../../lua/type/tes3apparatus.html
