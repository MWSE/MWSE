--[[
	Button Setting for binding a mouse button + modifier key combination. Variable returned in the form:
		{
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

--- @class mwseMCMMouseBinder
local MouseBinder = Parent:new()
MouseBinder.allowButtons = true

--- @param data mwseMCMMouseBinder.new.data|nil
--- @return mwseMCMMouseBinder
function MouseBinder:new(data)
	local t = Parent:new(data)

	setmetatable(t, self)
	self.__index = self
	--- @cast t mwseMCMMouseBinder

	local bothDisabled = (not t.allowButtons) and (not t.allowWheel)
	assert(bothDisabled ~= true, "[MouseBinder]: Both allowButtons and allowWheel options are false. \z
		At least one needs to be enabled to use MouseBinder.")

	-- Set up where we are getting input from.
	t.observeEvents = {}
	if t.allowButtons then
		t.observeEvents[tes3.event.mouseButtonDown] = true
	end
	if t.allowWheel then
		t.observeEvents[tes3.event.mouseWheel] = true
	end
	return t
end

return MouseBinder
