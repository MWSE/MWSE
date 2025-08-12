local function onJump(e)
	-- Check if the actor that is trying to jump is the player.
	if e.mobile ~= tes3.mobilePlayer then return end

	-- Define the threshold at which we no longer want to be able to jump.
	-- This is set to 50% of the player's maximum encumbrance.
	local encumbranceThreshold = e.mobile.encumbrance.base * 0.5

	-- Check if the current encumbrance is higher than our defined threshold and prevent jumping.
	if e.mobile.encumbrance.current <= encumbranceThreshold then return end


	-- Show a message to the player to inform them that jumping is not possible.
	tes3.messageBox("You are unable to jump.")

	-- Block the vanilla jump logic and prevent other jump events from being triggered.
	return false
end
event.register(tes3.event.jump, onJump)