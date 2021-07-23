return {
	type = "method",
	description = [[Calculates the damage that would be inflicted to an actor after armor and/or resistance. Returns the actual damage done after armor mitigation and resistance, but before difficulty scaling.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "damage", type = "number", description = "The amount of damage to apply." },
			{ name = "applyArmor", type = "boolean", optional = true, description = "If armor should mitigate the incoming damage." },
			{ name = "resistAttribute", type = "number", optional = true, description = "The resistance attribute that is applied to the damage. It can reduce damage or exploit weakness. Uses values from tes3.effectAttributes." },
		},
	}},
	valuetype = "number",
}