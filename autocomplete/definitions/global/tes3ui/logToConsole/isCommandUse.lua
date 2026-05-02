
local function example()
	-- This will make "player->ModStrength 10" appear in the console coloured brown.
	-- It can't be selected by using up arrow key.
	tes3ui.logToConsole("player->ModStrength 10", false)

	-- This will make "player->ModWillpower 10" appear in the console coloured blue.
	-- It can be selected by using up arrow key, and when the enter is pressed,
	-- it will call that function.
	tes3ui.logToConsole("player->ModWillpower 10", true)

	tes3.messageBox("Done! Open the console to see the result.")
end

event.register(tes3.event.keyDown, example, { filter = tes3.scanCode.u })
