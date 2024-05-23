--- @type tes3uiElement, table
local myPage, myConfig
mwse.mcm.createSlider{
	parent = myPage,
	label = "My distance slider",
	variable = mwse.mcm.createTableVariable{ id = "distance", table = myConfig },
	convertToLabelValue = function(self, variableValue)
		local feet = variableValue / 22.1
		local meters = 0.3048 * feet
		if self.decimalPlaces == 0 then
			return string.format("%i ft (%.2f m)", feet, meters)
		end
		return string.format(
			-- if `decimalPlaces == 1, then this string will simplify to
			-- "%.1f ft (%.3f m)"
			string.format("%%.%uf ft (%%.%uf m)", self.decimalPlaces, self.decimalPlaces + 2),
			feet, meters
		)
	end,

	max = 22.1 * 10,    -- max is 10 feet
	step = 22.1,        -- increment by 1 foot
	jump = 22.1 * 5,
}
