cellChanged
====================================================================================================

The cellChanged event is occurs when the player changes cells. This might occur from going through a door, using intervention or recall spells, or from scripted repositioning.

Event Data
----------------------------------------------------------------------------------------------------

previousCell
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3cell`_. Read-only. The previous cell that the player came from. This will be nil when loading a game.

cell
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3cell`_. Read-only. The new cell that the player has entered.

.. _`tes3cell`: ../../lua/type/tes3cell.html
