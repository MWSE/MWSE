
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