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

PercentageSlider.min = 0.0
PercentageSlider.max = 1.0
PercentageSlider.step = 0.01
PercentageSlider.jump = 0.05

function PercentageSlider:convertToWidgetValue(variableValue)
	return (variableValue - self.min) * 10 ^ (2 + self.decimalPlaces)
end

function PercentageSlider:updateValueLabel()
	local labelElement = self.elements.label
	local percentageValue = 100 * self.variable.value
	if string.find(self.label, "%s", 1, true) then
		labelElement.text = string.format(self.label, percentageValue)
	else
		local s = "%s: %i%%"
		-- only include decimal places when we're supposed to
		if self.decimalPlaces > 0 then
			-- this will simplify to "%s: %.1f%%" (in the case where `decimalPlaces` == 1)
			s = string.format("%%s: %%.%uf%%%%", self.decimalPlaces)
		end
		labelElement.text = s:format(self.label, percentageValue)
	end
end

return PercentageSlider