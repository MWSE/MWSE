return {
	type = "function",
	description = [[This function is used to claim a unique spell effect name and id. This is needed before actually creating a new effect by calling tes3.addMagicEffect. A claimed effect id can be retrieved as: tes3.effect.effectName (just like any regular spell effect).]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "name", type = "string", description = "The name of the new spell effect. Must be unique. An error will be thrown if it's non-unique." },
			{ name = "id", type = "number", description = "A unique number representing the new spell effect. An error will be thrown if it's non-unique." },
		},
	}},
}
