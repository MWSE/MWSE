--[[
	PercentageSlider:
		A slider that displays percentages as whole numbers, but stores the values as decimal numbers.
		i.e., values get shown in the range [0,100], but are stored in the config as [0,1]
		A Slider Setting that supports decimal numbers with specified
		amount of decimal places. The underlying implementation has
		a slider in range (min * 10 ^ decimalPlaces, max * 10 ^ decimalPlaces),
		while the label above the slider converts that to the decimal.
]]--

--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

local Parent = require("mcm.components.settings.Slider")
local Setting = require("mcm.components.settings.Setting")

--- @class mwseMCMDecimalSlider : mwseMCMSlider
local PercentageSlider = Parent:new()
PercentageSlider.min = 0.0
PercentageSlider.max = 1.0
PercentageSlider.step = 0.01
PercentageSlider.jump = 0.05

function PercentageSlider:updateValueLabel()
	local newValue = "" ---@type string|number

	if self.elements.slider then
		newValue = self.elements.slider.widget.current + self.min
	end

	if string.find(self.label, "%s", 1, true) then
		self.elements.label.text = string.format(self.label, newValue)
	else
		self.elements.label.text = string.format("%s: %s%%", self.label, newValue)
	end


end

function PercentageSlider:update()
	self.variable.value = (self.elements.slider.widget.current + self.min) / 100
	-- Bypass Slider:update to avoid overwriting the variable with an unscaled value.
	Setting.update(self)
end

return PercentageSlider
