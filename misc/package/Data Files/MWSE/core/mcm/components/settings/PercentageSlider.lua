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

-- probably a bit weird to implement a decimalPlaces and not inherit from `DecimalSlider`,
--  but we have to overwrite all the funcationality of DecimalSlider anyway, and the default values
-- of a PercentageSlider are more consistent with the default values of `Slider`.

-- also, the decimal place conversions in the code are going to be a bit weird, because

-- (1) the user wants to store values in a range [0,1]
-- (2) the user wants values to be displayed in the range [0,100]
-- (3) sliders can only store integer values

-- so, assuming `min == 0`, the conversions are:
-- (1) <-> (2): scale by `100`
-- (1) <-> (3): scale by `10^(2 + decimalPlaces)`
-- (2) <-> (3): scale by `10^decimalPlaces`


PercentageSlider.decimalPlaces = 0


--- @param data mwseMCMDecimalSlider.new.data?
--- @return mwseMCMDecimalSlider slider
function PercentageSlider:new(data)
	-- make sure `decimalPlaces` is ok, then do parent behavior
	if data and data.decimalPlaces ~= nil then
		assert(
			data.decimalPlaces >= 0 and data.decimalPlaces % 1 == 0, 
			"Invalid 'decimalPlaces' parameter provided. It must be a nonnegative whole number."
		)
	end
	---@diagnostic disable-next-line: param-type-mismatch, return-type-mismatch
	return Parent.new(self, data) -- the `__index` metamethod will make the `min`, `max`, etc fields default to the values specified above.
end

function PercentageSlider:getNewValue()
	return (self.elements.slider.widget.current / 100  + self.min) / 10^self.decimalPlaces
end

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

function PercentageSlider:getCurrentWidgetValue()
	return self.variable.value * 10^(2 + self.decimalPlaces) - self.min^self.decimalPlaces
end



function PercentageSlider:updateValueLabel()
	local newValue = "" ---@type string|number

	if self.elements.slider then
		-- this is the (2) <-> (3) conversion mentioned earlier, so we scale by `10^decimalPlaces`
		newValue = (self.min + self.elements.slider.widget.current) / 10^self.decimalPlaces
	end
	if string.find(self.label, "%s", 1, true) then
		self.elements.label.text = string.format(self.label, newValue)
	else
		self.elements.label.text = string.format("%s: %s%%", self.label, newValue)
	end
end



return PercentageSlider
