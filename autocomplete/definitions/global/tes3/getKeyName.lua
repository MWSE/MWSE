return {
	type = "function",
	description = "Gets the name of a corresponding `tes3.scanCode`, using the appropriate GMSTs. The `keyName` returned by this function is the same `keyName` \z
		that would be used in the in-game Controls menu.\n\n\z
		\z
		For example, `tes3.getKeyName(tes3.scanCode.b)` will return `\"B\"`, and `tes3.getKeyName(tes3.scanCode.rshift)` will return `\"Right Shift\"`.",
	arguments = {{
		name = "keyCode",
		type = "tes3.scanCode",
	}},
	returns = {{ name = "keyName", type = "string|nil", description = "A string representation of the given `keyCode`." }},
}
