-- The following code can be tested in-game by pressing Alt + l or Alt + k

event.register(tes3.event.keyDown, function(e)
	if not tes3.isKeyEqual({ actual = e, expected = { keyCode = tes3.scanCode.l, isAltDown = true } }) then return end

	tes3.messageBox("mwscript.explodeSpell")
	---@diagnostic disable-next-line: deprecated
	mwscript.explodeSpell({
		reference = tes3.game.playerTarget,
		spell = "proj_trap_spell"
	})
end)

event.register(tes3.event.keyDown, function(e)
	if not tes3.isKeyEqual({ actual = e, expected = { keyCode = tes3.scanCode.k, isAltDown = true } }) then return end

	tes3.messageBox("tes3.cast")
	-- This will behave the same as mwscript.explodeSpell()
	tes3.cast({
		target = tes3.game.playerTarget,
		reference = tes3.game.playerTarget,
		spell = "proj_trap_spell",
	})
end)
