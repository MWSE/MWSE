loaded
====================================================================================================

The loaded event fires after the game has been successfully loaded. This event is preceded by the load event.

Event Data
----------------------------------------------------------------------------------------------------

quickload
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`boolean`_. Read-only. If true, the load came from the quickload system.

filename
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`string`_. Read-only. The filename of the save that was loaded, not including file extension.

newGame
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`boolean`_. Read-only. If true, a new game was started.

.. _`boolean`: ../../lua/type/boolean.html
.. _`string`: ../../lua/type/string.html
