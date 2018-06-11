
enterFrame
========================================================

The **enterFrame** event occurs at the start of every frame.

.. note:: See the `Event Guide`_ for more information on event data, return values, and filters.


Event Data
--------------------------------------------------------

delta
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
`Number`_. Read-only. The number of seconds since the last frame.

timestamp
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
`Number`_. Read-only. The number of seconds since the beginning of the 3rd Era.	(similar to the Unix Epoch)

menumode
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
`Boolean`_. Read-only. If the game is paused- in inventory, menu, etc.


Filter
--------------------------------------------------------
This event does not support filters.


.. _`Event Guide`: ../guide/events.html
.. _`Number`: ../type/lua/number.html
.. _`Boolean`: ../type/lua/boolean.html
