local function example(e)
		-- We only care about the player
	if (e.mobile.objectType ~= tes3.objectType.mobilePlayer) then
		return
	end

	local cell = tes3.mobilePlayer.cell

	if (cell.isInterior and not cell.hasWater) then
		return
	end

	local waterLevel = cell.waterLevel
	local headPosition = tes3.mobilePlayer.position.z + tes3.mobilePlayer.height

	local underwater = headPosition < waterLevel

	if not underwater then return end

	-- There is a 50 % chance that any Water Breathing effect will be removed from the player
	tes3.removeEffects({
		reference = tes3.player,
		chance = 50,
		effect = tes3.effect.waterBreathing,
	})
end

event.register(tes3.event.initialized, function ()
	event.register(tes3.event.calcMoveSpeed, example)
end)