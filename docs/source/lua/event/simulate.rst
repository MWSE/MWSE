
simulate
========================================================

The **simulate** event occurs at the start of every frame the game simulation continues, outside of menu mode.

.. note:: To recieve updates inside menu mode, use the `enterFrame`_ event.

.. note:: See the `Event Guide`_ for more information on event data, return values, and filters.


Event Data
--------------------------------------------------------

delta
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
`Number`_. Read-only. The number of seconds since the last frame.

timestamp
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
`Number`_. Read-only. The number of seconds since the beginning of the 3rd Era.	(similar to the Unix Epoch)


Filter
--------------------------------------------------------
This event does not support filters.


.. _`Event Guide`: ../guide/events.html
.. _`enterFrame`: enterFrame.html
.. _`Number`: ../type/lua/number.html
