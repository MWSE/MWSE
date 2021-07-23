key
====================================================================================================

The key event fires when a key up or key down input is detected. It is preferred that the keyDown and keyUp events are used instead.

Event Data
----------------------------------------------------------------------------------------------------

isSuperDown
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`number`_. Read-only. True if super (Windows key) is held.

keyCode
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`number`_. Read-only. The scan code of the key that raised the event.

isAltDown
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`number`_. Read-only. True if alt  is held.

isControlDown
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`number`_. Read-only. True if control is held.

pressed
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`boolean`_. Read-only. True if this is a key down event, false for a key up event.

Examples
----------------------------------------------------------------------------------------------------

Show a Message when Ctrl-Z is Pressed
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Displays a simple message when Z is pressed while control is held.

.. code-block:: lua

    function myOnKeyCallback(e)
        if( e.pressed and e.isControlDown ) then
            tes3.messageBox({ message = "You pressed Ctrl-Z, but you can't undo all your mistakes." })
        end
    end

    -- Filter by the scan code to get Z key presses only.
    event.register("key", myOnKeyCallback, { filter = tes3.scanCode.z } )


.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
