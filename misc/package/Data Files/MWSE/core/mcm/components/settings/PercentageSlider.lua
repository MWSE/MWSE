--[[
	PercentageSlider:
		A slider that displays percentages as whole numbers, but stores the values as decimal numbers.
		i.e., values get shown in the range [0,100], but are stored in the config as [0,1].
]]--

--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

local Parent = require("mcm.components.settings.Slider")



--- @class mwseMCMPercentageSlider
local PercentageSlider = Parent:new()
PercentageSlider.__index = PercentageSlider

PercentageSlider.min = 0.0
PercentageSlider.max = 1.0
PercentageSlider.step = 0.01
PercentageSlider.jump = 0.05

function PercentageSlider:convertToWidgetValue(variableValue)
	return (variableValue - self.min) * 10 ^ (2 + self.decimalPlaces)
end

function PercentageSlider:convertToLabelValue(variableValue)
	if self.decimalPlaces == 0 then
		return (100 * variableValue) .. "%"
	end
	return string.format(table.concat{"%.", self.decimalPlaces, "f%%"}, 100 * variableValue)
end

return PercentageSlider