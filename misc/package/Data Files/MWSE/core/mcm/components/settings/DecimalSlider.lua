--[[
	DecimalSlider:
		A Slider Setting that supports decimal numbers with specified
		amount of decimal places. The underlying implementation has
		a slider in range (min * 10 ^ decimalPlaces, max * 10 ^ decimalPlaces),
		while the label above the slider converts that to the decimal.
]]--

--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

local Parent = require("mcm.components.settings.Slider")

--- @deprecated
--- @class mwseMCMDecimalSlider : mwseMCMSlider
local DecimalSlider = Parent:new()
DecimalSlider.min = 0.0
DecimalSlider.max = 1.0
DecimalSlider.step = 0.01
DecimalSlider.jump = 0.05
DecimalSlider.decimalPlaces = 2


--- @param data mwseMCMSlider.new.data?
--- @return mwseMCMDecimalSlider slider
function DecimalSlider:new(data)
	-- make sure `decimalPlaces` is ok, then do parent behavior
	if data and data.decimalPlaces ~= nil then
		assert(
			data.decimalPlaces % 1 == 0 and data.decimalPlaces >= 0,
			"mcm.DecimalSlider: Invalid 'decimalPlaces' parameter provided. It must be a positive whole number."
		)
	end
	--- @diagnostic disable-next-line: param-type-mismatch, return-type-mismatch
	return Parent.new(self, data) -- the `__index` metamethod will make the `min`, `max`, etc fields default to the values specified above.
end

return DecimalSlider