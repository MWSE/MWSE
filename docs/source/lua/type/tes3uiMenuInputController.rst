tes3uiMenuInputController
====================================================================================================

A core came structure responsible for managing the input around menu elements.

Properties
----------------------------------------------------------------------------------------------------

`lastInputTime`_ (`number`_)
    The amount of seconds that have passed since the last time the input was updated.

`menuController`_ (`tes3uiMenuController`_)
    A handle back to the main menu controller.

`textInputFocus`_ (`tes3uiElement`_)
    The element that is currently focused for text input.

.. toctree::
    :hidden:

    tes3uiMenuInputController/lastInputTime
    tes3uiMenuInputController/menuController
    tes3uiMenuInputController/textInputFocus

.. _`lastInputTime`: tes3uiMenuInputController/lastInputTime.html
.. _`menuController`: tes3uiMenuInputController/menuController.html
.. _`textInputFocus`: tes3uiMenuInputController/textInputFocus.html

Methods
----------------------------------------------------------------------------------------------------

`flushBufferedTextEvents`_
    Clears the current text input. If no text has been passed to the current text focus, this will prevent it from doing so.

.. toctree::
    :hidden:

    tes3uiMenuInputController/flushBufferedTextEvents

.. _`flushBufferedTextEvents`: tes3uiMenuInputController/flushBufferedTextEvents.html

.. _`number`: ../../lua/type/number.html
.. _`tes3uiElement`: ../../lua/type/tes3uiElement.html
