
local function onCollision(e)
	local target = e.target
	if not target then
		return
	end

	if target.object.objectType == tes3.objectType.door then
		tes3.messageBox("Ouch!\nMaybe I should open it first?")
	end
end

event.register(tes3.event.collision, onCollision, { filter = ("PlayerSaveGame"):lower() })
