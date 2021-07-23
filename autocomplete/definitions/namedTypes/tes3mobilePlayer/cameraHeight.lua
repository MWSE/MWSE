return {
	type = "value",
	description = [[Access to the player camera height, used for both the first person and third person camera (specifically the orbit centre). It is normally placed at head height by the game, on loading, or when a race is selected. The camera height can be reset to its regular value by setting this property to nil.]],
	valuetype = "number",
	examples = {
		["shrink"] = {
			title = "Adjust the camera to match shrunken player scale",
		},
	},
}