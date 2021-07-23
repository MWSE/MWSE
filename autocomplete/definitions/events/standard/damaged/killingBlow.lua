
-- Print a message if player's arrow killed its target
local function onDamaged(e)
    -- We only care if the player did the damage
    if e.attackerReference ~= tes3.player then
        return
    end

    -- Check if the damage was caused by a projectile, but not by a spell, so it must be an arrow or a bolt
    if e.projectile ~= nil and e.magicSourceInstance == nil then

        -- Did the damage kill the target?
        if e.killingBlow == true then
            tes3.messageBox("Your arrow felled its target!")
        end
    end
end
event.register("damaged", onDamaged)