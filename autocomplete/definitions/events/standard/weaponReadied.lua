return {
	type = "event",
	description = "This event is called when a weapon is readied, and pairs with the weaponUnreadied event. It can be used to reliably tell if a specific weapon is readied for attack. This does not necessarily mean that the animation state has changed for the first time.",
	related = { "weaponReadied", "weaponUnreadied" },
	eventData = {
		["reference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The reference associated with the change in readied weapon.",
		},
		["weaponStack"] = {
			type = "tes3equipmentStack",
			readOnly = true,
			description = "The stack that contains the newly readied weapon and its associated data.",
		},
	},
}