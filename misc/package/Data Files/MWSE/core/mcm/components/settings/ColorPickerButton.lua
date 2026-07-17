--[[
	An button setting that displays currently selected color and has a button that opens color picker menu.
]]--

--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

local Parent = require("mcm.components.settings.ColorPicker")
local Setting = require("mcm.components.settings.Setting")

--- @class mwseMCMColorPickerButton
local PickerButton = Parent:new()
PickerButton.__index = PickerButton

--- @param newValue mwseColorATable
function PickerButton:setVariableValue(newValue)
	-- Make sure we don't create a reference to newValue table (which is usually self.variable.defaultSetting).
	self.variable.value = table.copy(newValue)
	local element = self.elements.preview
	local preview = element.widget --[[@as tes3uiColorPreview]]
	preview:setColor(newValue --[[@as mwseColorTable]], newValue.a)
	element:updateLayout()
	self:update()
end

function PickerButton:disable()
	Setting.disable(self)
	self.elements.button.widget.state = tes3.uiState.disabled
end

-- UI creation functions

--- @param parentBlock tes3uiElement
function PickerButton:createInnerContainer(parentBlock)
	Parent.createInnerContainer(self, parentBlock)
	local innerContainer = self.elements.innerContainer
	innerContainer.borderAllSides = 8
	innerContainer.flowDirection = tes3.flowDirection.leftToRight
end


--- @param parentBlock tes3uiElement
function PickerButton:makeComponent(parentBlock)
	local variable = self.variable --[[@as mwseMCMTableVariable]]
	local initialColor = variable.value or variable.defaultSetting or self.initialColor
	local initialAlpha = initialColor.a or self.initialAlpha

	local previewElement = parentBlock:createColorPreview({
		id = tes3ui.registerID("mwseMCMColorPickerButton_preview"),
		color = initialColor,
		alpha = initialAlpha,
		height = 32,
		width = 128,
	})
	local grow = parentBlock:createBlock({ id = tes3ui.registerID("mwseMCMColorPickerButton_grow") })
	grow.autoWidth = true
	grow.widthProportional = 1.0
	local button = parentBlock:createButton({
		id = tes3ui.registerID("mwseMCMColorPickerButton_button"),
		text = mwse.mcm.i18n("Choose")
	})
	button:registerAfter(tes3.uiEvent.mouseClick, function()
		if self:checkDisabled() then return end
		tes3ui.showColorPickerMenu({
			initialColor = self.variable.value,
			initialAlpha = self.variable.value.a,
			alpha = self.alpha,
			closeCallback = function(selectedColor, selectedAlpha)
				--- @cast selectedColor mwseColorATable
				selectedColor.a = selectedAlpha --[[@as number]]
				self:setVariableValue(selectedColor)
			end
		})
	end)
	self.elements.button = button
	self.elements.preview = previewElement
	table.insert(self.mouseOvers, self.elements.preview)
end

return PickerButton
