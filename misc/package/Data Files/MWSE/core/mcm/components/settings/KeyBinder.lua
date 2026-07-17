--[[
	Button Setting for binding a key/mouse combination. Variable returned in the form:
		{
			keyCode = tes3.scanCode|nil,
			mouseButton = integer|nil,
			mouseWheel = integer|nil,
			isAltDown = boolean|nil,
			isShiftDown = boolean|nil,
			isControlDown = boolean|nil,
		}
]]--

--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

local Parent = require("mcm.components.settings.Binder")

--- @class mwseMCMKeyBinder
local KeyBinder = Parent:new()
KeyBinder.__index = KeyBinder
KeyBinder.allowMouse = false

-- TODO: Implement flags for enabling the binding of mouse wheel or mouse buttons separately

--- @param data mwseMCMKeyBinder.new.data|nil
--- @return mwseMCMKeyBinder
function KeyBinder:new(data)
	local t = Parent.new(self, data)

	--- @cast t mwseMCMKeyBinder

	-- All KeyBinders observe keyboard input.
	t.observeEvents = { [tes3.event.keyDown] = true }

	-- Check if we also observe input from mouse.
	if t.allowMouse then
		t.observeEvents[tes3.event.mouseButtonDown] = true
		t.observeEvents[tes3.event.mouseWheel] = true
	end
	return t
end

return KeyBinder
