local function onJump(e)
	-- Check if the actor that is trying to jump is the player.
	if e.mobile ~= tes3.mobilePlayer then return end

	-- Define the fixed height of the jump.
	local jumpHeight = 300

	-- Multiply the normalized direction vector of the jump by the fixed jump height.
	-- This will make jumps initiated during movement behave like they normally do.
	e.velocity = e.velocity:normalized() * jumpHeight
end
event.register(tes3.event.jump, onJump)