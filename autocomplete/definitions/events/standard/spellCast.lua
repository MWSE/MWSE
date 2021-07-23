return {
	description = "This event is triggered just before a spell cast is resolved, at the end of the casting animation. It can control the success chance of the spell cast.",
	eventData = {
		["caster"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The caster of the spell.",
		},
		["source"] = {
			type = "tes3spell",
			readOnly = true,
			description = "The magic source.",
		},
		["castChance"] = {
			type = "number",
			description = "The percentage chance of the caster successfully casting the spell. May be modified. Setting it to 0 will cause the cast to fail.",
		},
		["weakestSchool"] = {
			type = "number",
			readOnly = true,
			description = "Of all the magic effects in the spell, there is a magic school which the caster has the lowest skill at casting. This school determines which skill will gain experience on a successful cast.",
		},
	},
	filter = "source",
}