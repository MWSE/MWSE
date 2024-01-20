--- This function returns `true` if the player is too close to the specified reference.
---@param reference tes3reference
---@return boolean
local function isPlayerTooClose(reference)
    return tes3.player:intersects(reference)
end
