return {
	type = "function",
	description = [[Registers a new armor slot. Adds a new place for armor pieces with a matching slot number to equip to.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "slot", type = "number", description = "Armor slot number. A number greater than 10 to configure a slot for." },
			{ name = "name", type = "string" },
			{ name = "weight", type = "number", default = "0", description = "A stand-in for the armor base weight value, typically controlled by a GMST (e.g. iHelmWeight)." },
			{ name = "scalar", type = "number", default = "0.1", description = "A multiplier with range 0.0-1.0 that controls how much of an item's armor value applies to a character's overall armor rating. For comparison, standard chest armor uses 0.3, helmets, greaves and pauldrons use 0.1, and gauntlets use 0.05." },
		},
	}},
	returns = "wasAdded",
	valuetype = "boolean",
}