load
====================================================================================================

The load event fires when the game is about to load. This event allows scripts to block loading.

Event Data
----------------------------------------------------------------------------------------------------

quickload
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`boolean`_. Read-only. If true, the load came from the quickload system.

filename
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`string`_. The filename of the save we want to load, not including file extension. Can be modified to redirect to another save.

newGame
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`boolean`_. Read-only. If true, a new game is being started.

.. _`boolean`: ../../lua/type/boolean.html
.. _`string`: ../../lua/type/string.html
