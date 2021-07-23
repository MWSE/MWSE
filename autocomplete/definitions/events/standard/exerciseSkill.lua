return {
	description = "This event is invoked whenever the player gains experience in a skill. The event can be blocked to prevent progress. Additionally, both the skill gaining experience and the progress gained can be changed.",
	eventData = {
		skill = {
			type = "number",
			readOnly = true,
			description = "The skill that is gaining experience. This can be changed to provide experience to a different skill.",
		},
		progress = {
			type = "number",
			description = "The amount of experience that skill is gaining. Note that experience is not on a scale of 1 to 100. This value is modifiable.",
		},
	},
	filter = "skill",
}