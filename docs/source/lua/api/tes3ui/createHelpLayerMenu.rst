tes3ui.createHelpLayerMenu
====================================================================================================

Creates a help layer menu. Help layer menus include notifications and tooltips that are always above the rest of the interface. The game realizes this using a separate menu root and set of functions.

Note, to create tooltips with the correct behaviour, use tes3ui.createTooltipMenu.

Unlike standard menus, help layer menus are always created with a fixed frame.

Returns
----------------------------------------------------------------------------------------------------

`tes3uiElement`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

id (`number`_)
    The menu’s ID. The menu can be later accessed by tes3ui.findHelpLayerMenu(id).

.. _`number`: ../../../lua/type/number.html
.. _`tes3uiElement`: ../../../lua/type/tes3uiElement.html
