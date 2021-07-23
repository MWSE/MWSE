return {
	description = "This event is invoked whenever the player naturally gains a new level a skill. This is typically through exercise, training, or reading books.",
	eventData = {
		["skill"] = {
			type = "number",
			readOnly = true,
			description = "The skill that gained a new level experience.",
		},
		["level"] = {
			type = "number",
			readOnly = true,
			description = "The new level of the skill.",
		},
        ["source"] = {
            type = "",
            --readOnly = true,
            description = "",
        },
	},
	filter = "skill",
}
