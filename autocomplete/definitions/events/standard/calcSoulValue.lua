return {
	description = "This event is raised when determining the value of a soul, unmodified by GMSTs. The value can be modified, or used to provide a soul value to NPCs who would normally not be allowed one.",
	eventData = {
		["actor"] = {
			type = "tes3creature|tes3npc",
			readOnly = true,
			description = "The actor whose soul is being evaluated.",
		},
		["value"] = {
			type = "number",
			description = "The value of the actor's soul.",
		},
	},
}