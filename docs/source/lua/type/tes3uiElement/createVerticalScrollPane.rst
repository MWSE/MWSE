createVerticalScrollPane
====================================================================================================

Creates a vertically scrolling pane. Useful as a list box.

    Custom widget properties:
        | `number`_ ``element.widget.positionY``: Vertical scroll offset in pixels.
        | `boolean`_ ``element.widget.scrollbarVisible``: Set if the scrollbar is displayed.

    Custom widget methods:
        | ``element.widget:contentsChanged()``: Call to update scroll bar slider and limits after adding or removing elements to the content container. Because content size depends on layout, this must be run after a menu:updateLayout(). Only required if the content size changes.

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
