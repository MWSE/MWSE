createButton
====================================================================================================

Creates a clickable button. Register the "mouseClick" event to capture a button press.
    
    Custom widget properties:
        | `number`_ ``element.widget.state``: Interaction state. 1 = normal, 2 = disabled, 4 = active. Controls which colour set to use for text.
        | `table`_ (float[3]) ``element.widget.idle``: Text colour for normal state, no mouse interaction.
        | `table`_ (float[3]) ``element.widget.over``: Text colour for normal state, on mouseOver.
        | `table`_ (float[3]) ``element.widget.pressed``: Text colour for normal state, on mouseDown.
        | `table`_ (float[3]) ``element.widget.idleDisabled``: Text colour for disabled state, no mouse interaction.
        | `table`_ (float[3]) ``element.widget.overDisabled``: Text colour for disabled state, on mouseOver.
        | `table`_ (float[3]) ``element.widget.pressedDisabled``: Text colour for disabled state, on mouseDown.
        | `table`_ (float[3]) ``element.widget.idleActive``: Text colour for active state, no mouse interaction.
        | `table`_ (float[3]) ``element.widget.overActive``: Text colour for active state, on mouseOver.
        | `table`_ (float[3]) ``element.widget.pressedActive``: Text colour for active state, on mouseDown.

Returns
----------------------------------------------------------------------------------------------------

`tes3uiElement`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

id (`number`_)
    Optional. A registered identifier to help find this element later.

.. _`number`: ../../../lua/type/number.html
.. _`tes3uiElement`: ../../../lua/type/tes3uiElement.html
