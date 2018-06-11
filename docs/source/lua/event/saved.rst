
saved
========================================================

The **saved** event fires after the game has successfully been saved. This event is preceded by the `save`_ event.

.. note:: See the `Event Guide`_ for more information on event data, return values, and filters.


Event Data
--------------------------------------------------------

saveName
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
`String`_. Read-only. The user facing name of the save, shown in the load/save game menu.

fileName
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
`String`_. Read-only. The filename of the save, not including file extension.


Filter
--------------------------------------------------------
This event does not support filters.


.. _`Event Guide`: ../guide/events.html
.. _`String`: ../type/lua/string.html
.. _`save`: save.html
