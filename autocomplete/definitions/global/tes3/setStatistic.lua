return {
	type = "function",
	description = [[Sets a statistic on a given actor. This should be used instead of manually setting values on the game structures, to ensure that events and GUI elements are properly handled. Either skill, attribute, or the statistic's property name must be provided.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3mobileActor|tes3reference|string" },
			{ name = "attribute", type = "tes3.attribute|integer", optional = true, description = "The attribute to set. Uses a value from [`tes3.attribute`](https://mwse.github.io/MWSE/references/attributes/)" },
			{ name = "skill", type = "tes3.skill|integer", optional = true, description = "The skill to set. Uses a value from [`tes3.skill`](https://mwse.github.io/MWSE/references/skills/)" },
			{ name = "name", type = "string", optional = true, description = "The property name of the statistic to set. The names can be taken from the properties of `tes3mobileNPC` or `tes3mobileCreature`. Useful for specifying health, magicka, fatigue or encumbrance." },
			{ name = "base", type = "number", optional = true, description = "If set, the base value will be set." },
			{ name = "current", type = "number", optional = true, description = "If set, the current value will be set." },
			{ name = "value", type = "number", optional = true, description = "If set, both the base and current value will be set." },
			{ name = "limit", type = "boolean", optional = true, default = false, description = "If set, the attribute won't rise above 100 or fall below 0." },
		},
	}},
}