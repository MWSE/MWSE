return {
	type = "function",
	description = [[Creates a new enchantment object, which will be stored as part of the current saved game.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "id", type = "string", optional = true, description = "The new object's ID. Must be unique if provided." },
			{ name = "castType", type = "number", optional = false, description = "The enchantment castType. See tes3.enchantmentType." },
			{ name = "chargeCost", type = "number", optional = false, description = "The new enchantment charge cost. Must be greater than 0." },
			{ name = "maxCharge", type = "number", optional = false, description = "The new enchantment maximum charge. Must be greater than 0" },
			{ name = "flags", type = "number", optional = true, description = "The new enchantment flags." },
			{ name = "objectFlags", type = "number", default = 0, description = "The object flags initially set. Force set as modified." }
		},
	}},
}
