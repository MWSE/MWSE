enterFrame
====================================================================================================

The enterFrame event occurs at the start of every frame, including when the game is paused or in menu mode.

Event Data
----------------------------------------------------------------------------------------------------

timestamp
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`number`_. Read-only. The number of hours since the beginning of the 3rd Era (similar to the Unix Epoch).

delta
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`number`_. Read-only. The number of seconds since the last frame.

menuMode
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`boolean`_. Read-only. If the game is paused- in the inventory or a menu, etc.

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
