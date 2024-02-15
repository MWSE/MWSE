return {
	type = "function",
	description = [[Causes a misc item to be recognized as a soul gem, so that it can be used for soul trapping.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "item", type = "tes3misc|string", description = "The item to recognize as a soul gem." },
		},
	}},
	examples = {
		["customSoulGem"] = {
			title = "Make Dwemer Tubes be treated as Soul gems. Also, make sure Fargoth's soul always ends up in one if the player has one avilable.",
		}
	},
	returns = {{ name = "wasAdded", type = "boolean" }},
}