return {
	type = "function",
	description = [[Creates a new instance of this type.]],
	arguments = {
		{ name = "rotation", optional = true, type = "tes3matrix33" },
		{ name = "translation",optional = true, type = "tes3vector3" },
		{ name = "scale",optional = true, type = "number" },
	},
	valuetype = "tes3transform",
}