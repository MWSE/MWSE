attack
====================================================================================================

This event is invoked whenever an actor makes an attack with their fists or a weapon, or a creature makes any attack. It occurs at the release time of the attack, such as the downstroke of a melee weapon or when an arrow is shot. Lockpicks and probes do not invoke this event.

Related events: `attackStart`_

Event Data
----------------------------------------------------------------------------------------------------

targetReference
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. A shortcut to the reference being attacked.

mobile
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3mobileActor`_. Read-only. The mobile actor making the attack.

reference
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. A shortcut to the reference that is attacking.

targetMobile
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3mobileActor`_. Read-only. The mobile actor being attacked.

Examples
----------------------------------------------------------------------------------------------------

Show a Message when the Player Attacks
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: lua

    local function myOnAttackCallback(e)
        -- Someone other than the player is attacking.
        if (e.reference ~= tes3.player) then
            return
        end

        -- We hit someone!
        if (e.targetReference ~= nil) then
            tes3.messageBox("You hit %s!", e.targetReference.object.name or e.targetReference.object.id)
        end
    end
    event.register("attack", myOnAttackCallback)


.. _`attackStart`: ../../lua/event/attackStart.html
.. _`tes3mobileActor`: ../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
