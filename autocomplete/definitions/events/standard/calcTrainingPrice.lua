return {
	description = "This event is called when determining the cost of training, and can be used to modify the given price.",
	eventData = {
		["mobile"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile actor of the trainer the player is interacting with.",
		},
		["reference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "mobile’s related reference.",
		},
		["basePrice"] = {
			type = "number",
			readOnly = true,
			description = "The price before adjustment.",
		},
		["price"] = {
			type = "number",
			description = "The adjusted price of the training.",
		},
		["skill"] = {
			type = "tes3statisticSkill",
			readOnly = true,
			description = "The ID of the skill to be trained.",
		},
		["skillId"] = {
			type = "number",
			readOnly = true,
			description = "The ID of the skill to be trained.",
		},
	},
}