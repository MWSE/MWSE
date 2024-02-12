return {
	type = "event",
	description = "The spellTick event happens every frame that an effect is active on a target. This can be used to aid in scripted spells, cancel active spells, or otherwise monitor spell activity on actors.",
	eventData = {
		["caster"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The caster of the spell.",
		},
		["deltaTime"] = {
			type = "number",
			readOnly = true,
			description = "The number of seconds since the last spell tick.",
		},
		["effect"] = {
			type = "tes3effect",
			readOnly = true,
			description = "The individual effect structure on source.",
		},
		["effectId"] = {
			type = "tes3.effect|integer",
			readOnly = true,
			description = "The magic effect ID that is being ticked. Maps to values in [`tes3.effect`](https://mwse.github.io/MWSE/references/magic-effects/) namespace.",
		},
		["effectIndex"] = {
			type = "integer",
			readOnly = true,
			description = "The index of the effect in source’s effects list.",
		},
		["effectInstance"] = {
			type = "tes3magicEffectInstance",
			readOnly = true,
			description = "The unique instance of the magic effect.",
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
		["target"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The target of the spell. For self-targeted spells, this matches caster.",
		},
	},
	filter = "source",
	blockable = true,
}