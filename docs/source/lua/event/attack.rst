
attack
========================================================

This event is invoked whenever an actor makes an attack with their fists or a weapon, or a creature makes any attack.

.. note:: Lockpicks and probes do not invoke this event.

.. note:: See the `Event Guide`_ for more information on event data, return values, and filters.

Event Data
--------------------------------------------------------

mobile
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`Reference`_. Read-only. The actor making the attack.

reference
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
`Reference`_. Read-only. A shortcut to the reference of *mobile*.

targetMobile
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
`Reference`_. Read-only. If the attack would hit a target (regardless of whether skill checks would actually cause it to hit), this is the actor being attacked.

targetReference
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
`Reference`_. Read-only. A shortcut to the reference of *targetMobile*.

Filter
--------------------------------------------------------
This event may be filtered by **reference**.

Examples
--------------------------------------------------------

Show a message when the player attacks, and the target if they'd hit someone.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: lua
    
    function myOnAttackCallback(e)
        --someone other than the player is attacking
        if (e.reference ~= tes3.getPlayerRef()) then
            return
        end
        
        --we hit someone!
        if (e.targetReference ~= nil) then
            tes3.messageBox({ message = "You hit " .. e.targetReference.id })
        end
    end
    event.register("attack", myOnAttackCallback)

.. _`Event Guide`: ../guide/events.html

.. _`Reference`: ../type/tes3/reference.html