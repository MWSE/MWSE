tes3inputController
====================================================================================================

A data structure, off of the world controller, that handles input.

Properties
----------------------------------------------------------------------------------------------------

`creationFlags`_ (`number`_)
    Read-only. A bit field representing device capabilities and settings.

`inputMaps`_ (`table`_)
    Read-only. The array-style access to input bindings.

`keyboardState`_ (`table`_)
    Read-only. The array-style access to raw key states.

`mouseState`_ (`tes3directInputMouseState`_)
    Read-only. The raw DirectInput mouse state.

`previousKeyboardState`_ (`table`_)
    Read-only. The array-style access to the previous frame's raw key states.

`previousMouseStatement`_ (`tes3directInputMouseState`_)
    Read-only. The raw DirectInput mouse state for the previous state.

.. toctree::
    :hidden:

    tes3inputController/creationFlags
    tes3inputController/inputMaps
    tes3inputController/keyboardState
    tes3inputController/mouseState
    tes3inputController/previousKeyboardState
    tes3inputController/previousMouseStatement

.. _`creationFlags`: tes3inputController/creationFlags.html
.. _`inputMaps`: tes3inputController/inputMaps.html
.. _`keyboardState`: tes3inputController/keyboardState.html
.. _`mouseState`: tes3inputController/mouseState.html
.. _`previousKeyboardState`: tes3inputController/previousKeyboardState.html
.. _`previousMouseStatement`: tes3inputController/previousMouseStatement.html

Methods
----------------------------------------------------------------------------------------------------

`isAltDown`_
    Returns true if either alt modifier key is pressed.

`isCapsLockActive`_
    Returns true if the caps lock mode is active.

`isControlDown`_
    Returns true if either control modifier key is pressed.

`isKeyDown`_ (`boolean`_)
    Performs a key down test for a given scan key code.

`isKeyPressedThisFrame`_ (`boolean`_)
    Checks to see if a given scan code is pressed, and wasn't pressed last frame.

`isKeyReleasedThisFrame`_ (`boolean`_)
    Checks to see if a given scan code is released, and was pressed last frame.

`isMouseButtonDown`_ (`boolean`_)
    Returns the mouse down state for a given mouse button.

`isMouseButtonPressedThisFrame`_ (`boolean`_)
    Returns true if a mouse button was up last frame, and down this frame.

`isMouseButtonReleasedThisFrame`_ (`boolean`_)
    Returns true if a mouse button was down last frame, and up this frame.

`isShiftDown`_
    Returns true if either shift modifier key is pressed. It does not take into account caps lock.

`isSuperDown`_
    Returns true if either super/windows modifier key is pressed.

`keybindTest`_ (`boolean`_)
    Performs a test for a given keybind, and optionally a transition state.

.. toctree::
    :hidden:

    tes3inputController/isAltDown
    tes3inputController/isCapsLockActive
    tes3inputController/isControlDown
    tes3inputController/isKeyDown
    tes3inputController/isKeyPressedThisFrame
    tes3inputController/isKeyReleasedThisFrame
    tes3inputController/isMouseButtonDown
    tes3inputController/isMouseButtonPressedThisFrame
    tes3inputController/isMouseButtonReleasedThisFrame
    tes3inputController/isShiftDown
    tes3inputController/isSuperDown
    tes3inputController/keybindTest

.. _`isAltDown`: tes3inputController/isAltDown.html
.. _`isCapsLockActive`: tes3inputController/isCapsLockActive.html
.. _`isControlDown`: tes3inputController/isControlDown.html
.. _`isKeyDown`: tes3inputController/isKeyDown.html
.. _`isKeyPressedThisFrame`: tes3inputController/isKeyPressedThisFrame.html
.. _`isKeyReleasedThisFrame`: tes3inputController/isKeyReleasedThisFrame.html
.. _`isMouseButtonDown`: tes3inputController/isMouseButtonDown.html
.. _`isMouseButtonPressedThisFrame`: tes3inputController/isMouseButtonPressedThisFrame.html
.. _`isMouseButtonReleasedThisFrame`: tes3inputController/isMouseButtonReleasedThisFrame.html
.. _`isShiftDown`: tes3inputController/isShiftDown.html
.. _`isSuperDown`: tes3inputController/isSuperDown.html
.. _`keybindTest`: tes3inputController/keybindTest.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`table`: ../../lua/type/table.html
.. _`tes3directInputMouseState`: ../../lua/type/tes3directInputMouseState.html
