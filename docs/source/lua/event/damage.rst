damage
====================================================================================================

The damage event triggers before an actor is damaged. The damage value can be modified, or the damage can be prevented completely by blocking the event.

Event Data
----------------------------------------------------------------------------------------------------

activeMagicEffect
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3magicEffect`_. Read-only. tes3magicEffect which caused damage. Can be nil.

attackerReference
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. attacker mobile's associated reference. Can be nil.

reference
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. mobile’s associated reference.

mobile
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3mobileActor`_. Read-only. The mobile actor that is taking damage.

magicSourceInstance
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3magicSourceInstance`_. tes3magicSourceInstance of a spell that caused damage. Can be nil.

projectile
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3mobileProjectile`_. Read-only. Projectile that dealt the damage. Can be nil.

source
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

damageSourceType. Read-only. The origin of damage. Values of this variable can be: "script", "fall", "suffocation", "attack", "magic", "shield" or nil.

attacker
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3mobileActor`_. Read-only. The mobile actor dealing the damage. Can be nil.

damage
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`number`_. The amount of damage done.

Examples
----------------------------------------------------------------------------------------------------

Change fall damage
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: lua


    -- Change fall damage if player is a Bosmer
    local function onDamage(e)
        -- We only care if the player took some damage
        if e.reference ~= tes3.player then
            return
        end

        -- Check if the damage was caused by a fall
        if e.source ~= "fall" then
            return
        end

        -- Check weather the player is a Bosmer
        if e.reference.object.race.id:lower() == "bosmer" then    -- This is the same as tes3.player.object.race.id:lower() == "bosmer"
            -- Taunt the player
            tes3.messageBox("Ha ha ha, you broke your leg Bosmer")

            -- Double the damage. He is Fargoth's cousin after all
            e.damage = e.damage * 2
        end
    end
    event.register("damage", onDamage)


.. _`number`: ../../lua/type/number.html
.. _`tes3magicEffect`: ../../lua/type/tes3magicEffect.html
.. _`tes3magicSourceInstance`: ../../lua/type/tes3magicSourceInstance.html
.. _`tes3mobileActor`: ../../lua/type/tes3mobileActor.html
.. _`tes3mobileProjectile`: ../../lua/type/tes3mobileProjectile.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
