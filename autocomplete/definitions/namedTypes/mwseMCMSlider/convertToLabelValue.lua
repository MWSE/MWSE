return {
	type = "method",
	description = [[This function specifies how values stored in the `variable` field should correspond to values displayed in the slider label.
The default behavior is to consistently format decimal places (i.e., if `decimalPlaces == 2`, make sure two decimal places are shown.)
This can be overwritten in the `createNewSlider` method, allowing for custom formatting of variable values.]],
	arguments = {
		{ name = "variableValue", type = "number" }
	},
    examples = {
		["DistanceSlider"] = {
			title = "DistanceSlider",
            description = "The following example shows how the `convertToLabelValue` parameter can be used to create a slider for a config setting that handles distances. \z
                The config setting will be stored using game units, but the displayed value will be in real-world units. Recall that 1 game unit corresponds to 22.1 feet, and 1 foot is 0.3048 meters."
		},
		["SkillSlider"] = {
			title = "SkillSlider",
            description = "Here is an (admittedly less practical) example to help highlight the different ways `convertToLabelValue` can be used. \z
                In this example, it will be used to create a slider that stores a `tes3.skill` constant in the config, and then displays the name of the corresponding skill."
		},
	},
	returns = {{
		name = "labelValue", type = "number|string"
	}}
}
