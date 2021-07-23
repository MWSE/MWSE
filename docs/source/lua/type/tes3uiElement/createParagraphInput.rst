createParagraphInput
====================================================================================================

Creates a multi-line text input element, with line wrapping on. To receive input the keyboard must be captured with ``tes3ui.acquireTextInput(element)``. Read the input with the ``text`` property. Write an initial value to edit by setting the ``text`` property.

    Custom widget properties:
        | `number`_ (integer) ``element.widget.lengthLimit``: Maximum input length. Default is ``1023``.

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
