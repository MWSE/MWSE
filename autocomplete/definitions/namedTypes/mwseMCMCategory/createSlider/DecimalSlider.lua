--- @type tes3uiElement, table
local myPage, myConfig
mwse.mcm.createSlider{
	parent = myPage,
	label = "My slider",
	variable = mwse.mcm.createTableVariable{ id = "someval", table = myConfig },
	min = 0.00,
	max = 1.00,
	step = 0.01,
	jump = 0.05,
	decimalPlaces = 2
}
