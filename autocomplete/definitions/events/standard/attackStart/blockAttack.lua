
-- In this example, we prevent the player
-- from starting any attacks.

---@param e attackStartEventData
local function onAttackStart(e)
	if e.reference == tes3.player then
		return false
	end
end

event.register(tes3.event.attackStart, onAttackStart)
