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

--[[ probably a bit weird to implement a `decimalPlaces` and not inherit from `DecimalSlider`,
	but we have to overwrite all the functionality of DecimalSlider anyway, and the default values
	of a `PercentageSlider` are more consistent with the default values of `Slider`
]]

--[[the decimal place conversions in the code are going to be a bit weird, because

	(1) the user wants to store values in a range [0,1]
	(2) the user wants values to be displayed in the range [0,100]
	(3) sliders can only store integer values

so, assuming `min == 0`, the conversions are:
	(1) <-> (2): scale by `100`
	(2) <-> (3): scale by `10^decimalPlace`

`min` opens up its own can of worms because the minimum value stored in the config should be `min / 100`
so, the config value will be getting scaled by `10 ^ (2 + decimalPlaces)`, 
while `min` will only be getting scaled by `10 ^ decimalPlaces`
]]

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
	return Parent.new(self, data) -- the `__index` metamethod will make the `min`, `max`, etc fields default to the values specified above.
end


function PercentageSlider:scaleToSliderRange(value)
	-- (2) -> (3) conversion
	return value * 10 ^ self.decimalPlaces
end

function PercentageSlider:scaleToVariableRange(value)
	-- (3) -> (2) conversion
	return value / 10 ^ self.decimalPlaces
end



function PercentageSlider:getCurrentWidgetValue()
	-- (1) -> (2) conversion
	-- note that the minimum value to store in the config is `self.min / 100`, 
	-- which is why `min` isn't getting  multiplying it by 100 here 
	local newValue = self.variable.value * 100 
	-- (2) -> (3) conversion
	return self:scaleToSliderRange(newValue - self.min)
end

--[[
	let x = variable, s = slider, m = min, d = decimalPlaces. then

	s = (100x - m) * 10^d
	
	~> 100x + m = s/10^d
	~> x = (s/10^d + m) / 100 
]]

function PercentageSlider:getNewValue()
	-- (3) -> (2) conversion
	local newValue = self.min + self:scaleToVariableRange(self.elements.slider.widget.current)
	-- (2) -> (1) conversion
	return newValue / 100
end


function PercentageSlider:updateValueLabel()
	local newValue = 0 ---@type string|number

	if self.elements.slider then
		-- (3) -> (1) -> (2) conversion
		newValue = self:getNewValue() * 100
	end
	if string.find(self.label, "%s", 1, true) then
		self.elements.label.text = string.format(self.label, newValue)
	else
		local s = "%s: %i%%"
		-- only include decimal places when we're supposed to
		if self.decimalPlaces > 0 then
			-- so sorry that anyone has to look at this
			-- this will simplify to "%s: %.1f%%" (in the case where `decimalPlaces` == 1)
			s = string.format("%%s: %%.%uf%%%%", self.decimalPlaces)
		end
		self.elements.label.text = s:format(self.label, newValue)
	end
end



return PercentageSlider
