return {
	description = "This event is called when a weapon is readied, and pairs with the weaponUnreadied event. It can be used to reliably tell if a specific weapon is readied for attack. This does not necessarily mean that the animation state has changed for the first time.",
	eventData = {
		["reference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The reference associated with the change in readied weapon.",
		},
		["weaponStack"] = {
			type = "tes3baseObject",
			readOnly = true,
			description = "The bundle that contains the newly readied weapon and its associated data.",
		},
	},
}