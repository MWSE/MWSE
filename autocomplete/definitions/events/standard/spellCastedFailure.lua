return {
	description = "This event is triggered when any spell fails to cast due to failing the cast chance check. It does not trigger when there is insufficient magicka.",
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
		["source"] = {
			type = "tes3spell",
			readOnly = true,
			description = "The magic source.",
		},
		["sourceInstance"] = {
			type = "tes3magicSourceInstance",
			readOnly = true,
			description = "The unique instance of the magic source.",
		},
		["expGainSchool"] = {
			type = "number",
			description = "Of all the magic effects in the spell, there is a magic school which the caster has the lowest skill at casting. This school determines which skill will gain experience on a successful cast. This school can be altered, or set to nil to remove experience gain, possibly in favour of your own experience calculation.",
		},
	},
	filter = "source",
}