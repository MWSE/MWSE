-- This variable is used to store a function that will be
-- registered as a callback when a button is pressed inside
-- our messageBox. The function's body is declared later.
local onButtonPressed


-- This function is registered to execute when U key is pressed.
local function onKeyPressed()
	tes3.messageBox({
		message = "Do you want a muffin?",
		buttons = { "Yes", "No" },
		showInDialog = false,
		callback = onButtonPressed,
	})
end
event.register(tes3.event.keyDown, onKeyPressed, { filter = tes3.scanCode.u })

-- Here we define the body of our function.
onButtonPressed = function(e)
	-- This corresponds to the first button
	-- of our message, which is "Yes"
	if e.button == 0 then
		tes3.addItem({
			reference = tes3.player,
			item = "ingred_bread_01_UNI3",
		})
	end
end
