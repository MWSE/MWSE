return {
	description = "This event is invoked whenever an actor makes an attack with their fists or a weapon, or a creature makes any attack. Lockpicks and probes do not invoke this event.",
	eventData = {
		["mobile"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile actor making the attack.",
		},
		["reference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "A shortcut to the reference that is attacking.",
		},
		["targetMobile"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile actor being attacked.",
		},
		["targetReference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "A shortcut to the reference being attacked.",
		},
	},
	filter = "reference",
	examples = {
		["ShowPlayerAttack"] = {
			title = "Show a Message when the Player Attacks",
		},
	},
}