return {
	type = "function",
	description = [[Calculates the chance that a caster can cast a given spell.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "checkMagicka", type = "boolean", optional = true, description = "Determines if the caster's magicka should be taken into account during the calculation." },
			{ name = "caster", type = "tes3reference|tes3mobileActor", optional = false, description = "The caster to perform the calculation against." },
        },
	}},
	valuetype = "number",
}
