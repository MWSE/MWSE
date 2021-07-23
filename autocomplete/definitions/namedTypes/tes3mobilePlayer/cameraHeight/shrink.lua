local function shrink()
	-- Shrink player.
	tes3.player.scale = 0.5
	-- Reset camera height to default value.
	tes3.mobilePlayer.cameraHeight = nil
	-- Modify camera height to match scale.
	tes3.mobilePlayer.cameraHeight = tes3.player.scale * tes3.mobilePlayer.cameraHeight
end

local function unshrink()
	tes3.player.scale = 1
	tes3.mobilePlayer.cameraHeight = nil
end