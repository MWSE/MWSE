return {
	type = "method",
	description = [[This function performs a check for presence of a given mobile actor.]],
	arguments = {
		{ name = "actor", type = "tes3mobileActor", description = "The actor to perform a check for." },
		{ name = "ignoreCreatures", type = "boolean", optional = true, default = true },
	},
	valuetype = "boolean"
}
