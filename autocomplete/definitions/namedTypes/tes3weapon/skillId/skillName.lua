
local function showMessage(e)
	if not e.mobile.readiedWeapon then return end
	local id = e.mobile.readiedWeapon.object.skillId
	local name = tes3.getSkillName(id)

	tes3.messageBox(name)
end

event.register(tes3.event.attackStart, showMessage)
