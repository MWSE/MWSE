uiShowRestMenu
====================================================================================================

The uiShowRestMenu event is raised when the rest menu is about to be displayed. It allows the callback to change if sleeping is allowed, or to prevent the UI from showing at all.

Event Data
----------------------------------------------------------------------------------------------------

allowRest
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`boolean`_. true if the player is allowed to sleep (determined primarily from the cell flags), or false if they must wait instead. This value can be modified.

scripted
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`boolean`_. Read-only. This value is true when the menu is shown from using the ShowRestMenu or equivalent function.

.. _`boolean`: ../../lua/type/boolean.html
