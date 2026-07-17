--[[
	An inline color picker setting.
]]--

--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

local format = require("mwse.ui.tes3uiElement.createColorPicker.formatHelpers")
local UIID = require("mwse.ui.tes3uiElement.createColorPicker.uiid")

local Parent = require("mcm.components.settings.Setting")

--- @class mwseMCMColorPicker
local ColorPicker = Parent:new()
ColorPicker.__index = ColorPicker
ColorPicker.initialColor = { r = 1.0, g = 1.0, b = 1.0 }
ColorPicker.initialAlpha = 1.0

--- @param newValue mwseColorATable
function ColorPicker:setVariableValue(newValue)
	-- Make sure we don't create a reference to newValue table (which is usually self.variable.defaultSetting).
	self.variable.value = table.copy(newValue)
	local parent = self.elements.picker
	local picker = parent.widget --[[@as tes3uiColorPicker]]
	picker:hueChanged(newValue --[[@as mwseColorTable]], newValue.a)
	self:update()
end

--- Updates the value stored in the variable. Doesn't update the widget.
--- @param newValue mwseColorATable
function ColorPicker:updateVariableValue(newValue)
	-- Make sure we don't create a reference to newValue table.
	self.variable.value = table.copy(newValue)
	self:update()
end

--- @param element tes3uiElement
--- @param eventID tes3.uiEvent
--- @param callback fun(e: tes3uiEventData): boolean?
local function registerRecursive(element, eventID, callback)
	element:register(eventID, callback)
	for _, child in ipairs(element.children) do
		registerRecursive(child, eventID, callback)
	end
end

-- nop
local function blockInteraction() end

-- An array of all the events used in the ColorPicker widget implementation.
local blockedEvents = {
	tes3.uiEvent.mouseDown,
	tes3.uiEvent.mouseRelease,
	tes3.uiEvent.mouseStillPressed,
	tes3.uiEvent.partScrollBarChanged,
	tes3.uiEvent.keyEnter,
	tes3.uiEvent.mouseClick,
}

function ColorPicker:disable()
	Parent.disable(self)

	local parent = self.elements.picker
	for _, eventID in ipairs(blockedEvents) do
		registerRecursive(parent, eventID, blockInteraction)
	end

	-- Also disable the value input below the main picker.
	local textInputContainer = parent:findChild(UIID.dataRowContainer)
	textInputContainer.visible = false
end

function ColorPicker:convertToLabelValue(variableValue)
	return "#" .. format.pixelToHex(variableValue)
end

-- UI creation functions

--- @param parentBlock tes3uiElement
function ColorPicker:makeComponent(parentBlock)
	local variable = self.variable --[[@as mwseMCMTableVariable]]
	local initialColor = variable.value or variable.defaultSetting or self.initialColor
	local initialAlpha = initialColor.a or self.initialAlpha

	local pickerElement = parentBlock:createColorPicker({
		id = tes3ui.registerID("mwseMCMColorPicker"),
		initialColor = initialColor,
		alpha = self.alpha,
		initialAlpha = initialAlpha,
		showOriginal = true,
		showDataRow = true,
		vertical = self.vertical,
	})
	pickerElement.borderAllSides = 8
	-- Make sure our variable stays in sync with the currently picked color.
	pickerElement:register( tes3.uiEvent.colorChanged, function()
		local picker = pickerElement.widget --[[@as tes3uiColorPicker]]
		self:updateVariableValue(picker:getRGBA())
	end)

	self.elements.picker = pickerElement
	self:insertMouseovers(pickerElement)
end


return ColorPicker
