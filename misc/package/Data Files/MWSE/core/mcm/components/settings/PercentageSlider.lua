--[[
	PercentageSlider:
		A slider that displays percentages as whole numbers, but stores the values as decimal numbers.
		i.e., values get shown in the range [0,100], but are stored in the config as [0,1].
]]--

--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

local Parent = require("mcm.components.settings.Slider")



--- @class mwseMCMPercentageSlider : mwseMCMSlider
---@field decimalPlaces integer
local PercentageSlider = Parent:new()

PercentageSlider.decimalPlaces = 0


--- @param data mwseMCMPercentageSlider.new.data?
--- @return mwseMCMPercentageSlider slider
function PercentageSlider:new(data)
	-- make sure `decimalPlaces` is ok, then do parent behavior
	-- unlike `DecimalSlider`, here we allow `decimalPlaces` to be `>= 0`
	if data and data.decimalPlaces ~= nil then
		assert(
			data.decimalPlaces >= 0 and data.decimalPlaces % 1 == 0, 
			"Invalid 'decimalPlaces' parameter provided. It must be a nonnegative whole number."
		)
	end
	---@diagnostic disable-next-line: param-type-mismatch, return-type-mismatch
	return Parent.new(self, data)
end


function PercentageSlider:convertToVariableValue(sliderValue)
	return (self.min + sliderValue ^ self.decimalPlaces) / 100

end

function PercentageSlider:convertToSliderValue(variableValue)
	return (100 * variableValue - self.min) * 10 ^ self.decimalPlaces
end

function PercentageSlider:updateValueLabel()
	local labelElement = self.elements.label

	if string.find(self.label, "%s", 1, true) then
		labelElement.text = string.format(self.label, self.variable.value)
	else
		local s = "%s: %i%%"
		-- only include decimal places when we're supposed to
		if self.decimalPlaces > 0 then
			-- so sorry that anyone has to look at this
			-- this will simplify to "%s: %.1f%%" (in the case where `decimalPlaces` == 1)
			s = string.format("%%s: %%.%uf%%%%", self.decimalPlaces)
		end
		labelElement.text = s:format(self.label, self.variable.value)
	end
end



return PercentageSlider