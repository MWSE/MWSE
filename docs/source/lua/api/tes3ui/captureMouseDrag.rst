tes3ui.captureMouseDrag
====================================================================================================

When used in a mouse event, causes the element to capture further mouse events even when the cursor goes outside the element. Setting mouse capture should always be accompanied by releasing it on a complementary event. This is commonly used for dragging, so that it can continue even if the mouse moves slightly outside the element.

The capture is always removed when the element is destroyed. The capture may also be removed when the mouse is released, but this is not reliable, as the engine forgets what to do if there is input from any other controller while the mouse is held down.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

capture (`boolean`_)
    Turns on mouse capture for the element currently processing a mouse event if true, sending all further mouse events to that element. Turns off capture if false.

Examples
----------------------------------------------------------------------------------------------------

Basic Usage
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Using the UI event system, cause a given element to capture the mouse drag.

.. code-block:: lua

    element:register("mouseDown", function(e)
        -- Capture must be inside a mouse event.
        tes3ui.captureMouseDrag(true)
    end)

    element:register("mouseRelease", function(e)
        -- Release may be anywhere.
        tes3ui.captureMouseDrag(false)
    end)


.. _`boolean`: ../../../lua/type/boolean.html
