createTextSelect
====================================================================================================

Creates a selectable line of text, with configurable hover, click, and disabled colours. Can be used to create a list box by placing them in a ScrollPane. ``state`` sets the initial interaction state, documented below.

    Custom widget properties:
        | `number`_ ``element.widget.state``: Interaction state. 1 = normal, 2 = disabled, 4 = active. Controls which colour set to use.
        | `table`_ (float[3]) ``element.widget.idle``: Colour for normal state, no mouse interaction.
        | `table`_ (float[3]) ``element.widget.over``: Colour for normal state, on mouseOver.
        | `table`_ (float[3]) ``element.widget.pressed``: Colour for normal state, on mouseDown.
        | `table`_ (float[3]) ``element.widget.idleDisabled``: Colour for disabled state, no mouse interaction.
        | `table`_ (float[3]) ``element.widget.overDisabled``: Colour for disabled state, on mouseOver.
        | `table`_ (float[3]) ``element.widget.pressedDisabled``: Colour for disabled state, on mouseDown.
        | `table`_ (float[3]) ``element.widget.idleActive``: Colour for active state, no mouse interaction.
        | `table`_ (float[3]) ``element.widget.overActive``: Colour for active state, on mouseOver.
        | `table`_ (float[3]) ``element.widget.pressedActive``: Colour for active state, on mouseDown.

Returns
----------------------------------------------------------------------------------------------------

`tes3uiElement`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

id (`number`_)
    Optional. A registered identifier to help find this element later.

text (`string`_)
    Optional. The text to display.

state (`number`_)
    Optional. The initial interaction state. Defaults to normal.

.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3uiElement`: ../../../lua/type/tes3uiElement.html
