--[[
	CycleButton: Toggles a variable between given options.
	It has the same use as Dropdown, but it looks different.

]]--

--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

-- Parent class
local Parent = require("mcm.components.settings.Button")

--- Class object
--- @class mwseMCMCycleButton
local CycleButton = Parent:new()

--- @param parentBlock tes3uiElement
function CycleButton:makeComponent(parentBlock)
	local button = parentBlock:createCycleButton({
		options = self.options
	})
	button.borderAllSides = 0
	button.borderRight = self.indent
	button.autoWidth = true

	local widget = button.widget --[[@as tes3uiCycleButton]]
	if self:checkDisabled() then
		local textElement = widget:getTextElement()
		textElement.text = self.disabledText
	else
		widget.value = self.variable.value
	end

	button:registerAfter("valueChanged", function(e)
		self.variable.value = e.source.widget.value
		self:update()
	end)

	-- Prevent clicking if disabled
	button:registerBefore(tes3.uiEvent.mouseClick, function(e)
		if self:checkDisabled() then
			return true
		end
		e.source:forwardEvent(e)
	end)
	self.elements.button = button
	table.insert(self.mouseOvers, button)
end

function CycleButton:enable()
	self.elements.label.color = tes3ui.getPalette("normal_color")
end

function CycleButton:disable()
	self.elements.label.color = tes3ui.getPalette("disabled_color")
end

function CycleButton:getText() end

function CycleButton:setText() end

function CycleButton:press() end

return CycleButton
