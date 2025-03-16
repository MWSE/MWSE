--[[
	Info field that shows mouseover information for settings
]]--

--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

local Parent = require("mcm.components.infos.Info")

--- @class mwseMCMMouseOverInfo
local MouseOverInfo = Parent:new()
MouseOverInfo.triggerOn = "MCM:MouseOver"
MouseOverInfo.triggerOff = "MCM:MouseLeave"


-- This function updates the `text` in this `Info` to be either the
-- `description` of the `Component`, or the `self.text` parameter of the `Info`.
--- @param component mwseMCMComponent|nil
function MouseOverInfo:updateInfo(component)

	-- If component has a description, update mouseOver
	-- Or return to original text on mouseLeave
	local newText = component and component:getMouseOverText() or self.text or ""
	self.elements.info.text = newText
	self:update()
end

--- @param parentBlock tes3uiElement
function MouseOverInfo:makeComponent(parentBlock)
	Parent.makeComponent(self, parentBlock)

	--- @param e { component: mwseMCMMouseOverInfo }
	local function updateInfo(e)
		self:updateInfo(e.component)
	end

	-- Register events
	event.register(self.triggerOn, updateInfo)
	event.register(self.triggerOff, updateInfo)
	parentBlock:register(tes3.uiEvent.destroy, function()
		event.unregister(self.triggerOn, updateInfo)
		event.unregister(self.triggerOff, updateInfo)
	end)
	self:updateInfo()
end

return MouseOverInfo
