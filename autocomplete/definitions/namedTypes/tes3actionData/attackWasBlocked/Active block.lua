event.register(tes3.event.keyDown, function(e)
	if e.keyCode ~= tes3.scanCode.g then return end

	-- Start block anim.
	local animRefr = tes3.mobilePlayer.is3rdPerson and tes3.player1stPerson or tes3.player
	tes3.playAnimation({
		reference = animRefr,
		shield = tes3.animationGroup.shield,
		loopCount = 0
	})

	-- Pretend blocking state was triggered.
	-- When set, the controller will clean up the anim after it completes. Resets to 0 after clean up.
	tes3.mobilePlayer.actionData.blockingState = 2
end)

---@param e attackHitEventData
event.register(tes3.event.attackHit, function(e)
	local target = e.targetMobile
	if not target then return end

	local isBlocking = target.actionData.blockingState > 0

	if target.readiedShield and isBlocking then
		-- This blocks hits and redirects damage to the shield.
		-- If there is no shield, it still blocks damage, so check for a shield first.
		e.mobile.actionData.attackWasBlocked = true
	end
end)
