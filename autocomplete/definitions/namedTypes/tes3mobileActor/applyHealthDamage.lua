return {
	type = "method",
	description = [[**Deprecated, please use applyDamage instead.**

Damages the actor.]],
	arguments = {
		{ name = "damage", type = "number", description = "The amount of damage to apply." },
		{ name = "isPlayerAttack", type = "boolean", description = "Used to calculate difficulty scaling." },
		{ name = "scaleWithDifficulty", type = "boolean", description = "Apply difficulty scaling to the damage." },
		{ name = "doNotChangeHealth", type = "boolean", description = "Do all the audiovisual effects of damage without the health change." },
	},
	valuetype = "boolean",
}