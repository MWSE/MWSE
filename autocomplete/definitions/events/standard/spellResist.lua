return {
	description = "This event is used when calculating a creature's spell resistance, and allows Lua scripts to override the behavior of spell resistance by changing the resistedPercent value. This can be used to enable willpower-based resistance checks, provide specific resistances to specific spells, spells that heal instead of harm, and a variety of new mechanics.",
	eventData = {
		["caster"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The caster of the spell.",
		},
		["target"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The target of the spell. For self-targeted spells, this matches caster.",
		},
		["resistedPercent"] = {
			type = "number",
			description = "The percent of the spell that has been resisted. This can be modified, but a value outside the range of 0 to 100 does not have consistent effects. For fire damage, for example, a value over 100 causes the spell to heal the target instead of harming them.",
		},
		["resistAttribute"] = {
			type = "number",
			readOnly = true,
			description = "The attribute resisted. This is an index into a tes3mobileActor.effectAttributes. Note that the index here is 0-based, while Lua is 1-based.",
		},
		["source"] = {
			type = "tes3alchemy|tes3enchantment|tes3spell",
			readOnly = true,
			description = "The magic source.",
		},
		["sourceInstance"] = {
			type = "tes3magicSourceInstance",
			readOnly = true,
			description = "The unique instance of the magic source.",
		},
		["effectIndex"] = {
			type = "number",
			readOnly = true,
			description = "The index of the effect in source's effects list.",
		},
		["effectInstance"] = {
			type = "tes3magicEffectInstance",
			readOnly = true,
			description = "The unique instance of the magic effect.",
		},
	},
	filter = "source",
}