save
====================================================================================================

The save event fires when the game is about to save. This event allows scripts to block saving.

Related events: `saved`_

Event Data
----------------------------------------------------------------------------------------------------

name
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`string`_. The user facing name of the save, shown in the load/save game menu.

filename
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`string`_. The filename of the save, not including file extension.

.. _`saved`: ../../lua/event/saved.html
.. _`string`: ../../lua/type/string.html
