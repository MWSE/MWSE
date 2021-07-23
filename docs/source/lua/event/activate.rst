activate
====================================================================================================

This event is invoked whenever something is activated, typically by the player. Activation is usually done with the associated activate/use key, but may also be forced by scripts.

Non-player actors may also activate objects, such as when opening doors, or via MWSE functions like `xActivate`_ or ``tes3.activate()``.

Some examples of when the activate event fires includes:

- When a door is used.
- When an item is picked up.
- When someone attempts to open a container.

Event Data
----------------------------------------------------------------------------------------------------

activator
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. The actor attempting to trigger the event.

target
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. The reference that is being activated.

Examples
----------------------------------------------------------------------------------------------------

Show a message for what the player is activating
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: lua

    -- Show the id when the player activates an object.
    local function myOnActivateCallback(e)
        if (e.activator == tes3.player) then
            tes3.messageBox({ message = "Activated " .. e.target.object.id })
        end
    end
    event.register("activate", myOnActivateCallback)


Prevent the player from activating NPCs
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: lua


    -- Show the id when the player activates an object.
    local function myOnActivateCallback(e)
        -- We only care if the PC is activating something.
        if (e.activator ~= tes3.player) then
            return
        end

        -- If the player targets an NPC, block the activation.
        if (e.target.object.objectType == tes3.objectType.npc) then
            return false
        end
    end
    event.register("activate", myOnActivateCallback)


.. _`xActivate`: ../../mwscript/functions/actor/xActivate.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
