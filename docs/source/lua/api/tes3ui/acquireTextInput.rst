tes3ui.acquireTextInput
====================================================================================================

Sends all text input to the specified element.  Calling this function with a nil argument will release text input and allow keybinds to work. Suppresses most keybinds while active, except the Journal open/close keybind (it's up to the individual menu implementation).

Only one element can have control of input, and there is no automatic restoration of control if one element takes control from another. Be careful of conflicts with other users of this function.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

element (`tes3uiElement`_)
    Optional. No description available.

.. _`tes3uiElement`: ../../../lua/type/tes3uiElement.html
