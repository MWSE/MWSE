--[[
	YesNoButton: Toggles a variable between true and false, showing "On" or "Off"
	in the button text

]]--

--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

local Parent = require("mcm.components.settings.Button")

--- @class mwseMCMYesNoButton
local YesNoButton = Parent:new()
YesNoButton.defaultSetting = false

function YesNoButton:getText()
	local text = (self.variable.value and tes3.findGMST(tes3.gmst.sYes).value or tes3.findGMST(tes3.gmst.sNo).value)
	return text --[[@as string]]
end

function YesNoButton:press()
	-- Toggle variable
	self.variable.value = not self.variable.value
	-- Do this after changing the variable so the callback is correct, and the text is updated
	self:update()
end

return YesNoButton
