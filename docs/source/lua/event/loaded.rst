
loaded
========================================================

The **loaded** event fires when the player leaves menu mode. This event is preceded by the `load`_ event.

.. note:: See the `Event Guide`_ for more information on event data, return values, and filters.


Event Data
--------------------------------------------------------

fileName
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
`String`_. Read-only. The filename of the save we want to load, not including file extension.

quickLoad
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
`Boolean`_. Read-only. True if this load was caused by pressing the quickload key. 

newGame
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
`Boolean`_. Read-only. True if we're starting a new game.


Filter
--------------------------------------------------------
This event does not support filters.


.. _`Event Guide`: ../guide/events.html
.. _`String`: ../type/lua/string.html
.. _`Boolean`: ../type/lua/boolean.html
.. _`load`: load.html
