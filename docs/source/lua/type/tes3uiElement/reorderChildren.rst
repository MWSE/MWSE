reorderChildren
====================================================================================================

    Moves the layout order of the children of this element. ``count`` elements are taken from starting child `Element`_ or index (0-based) ``moveFrom``, and moved before the child `Element`_ or index (0-based) ``insertBefore``. If ``count`` is -1, all children after ``moveFrom`` are moved. If any index is a negative number, then the index represents a distance from the end of the child list.
    
    Returns ``true`` if the operation succeeded, or ``false`` if at least one argument was invalid.

Returns
----------------------------------------------------------------------------------------------------

`boolean`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

insertBefore (`tes3uiElement`_, `number`_)
    The insertion point (or its 0-based child index).

moveFrom (`tes3uiElement`_, `number`_)
    The first child (or 0-based child index) to be moved.

count (`number`_)
    The property name.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`number`: ../../../lua/type/number.html
.. _`tes3uiElement`: ../../../lua/type/tes3uiElement.html
