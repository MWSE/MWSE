return {
	description = "This event is called when a Mobile Actor is activated. This may be the first time that a given Reference has an associated Mobile Actor attached to it. Typically this happens when transitioning through cells. When the player enters a cell, the mobileActivated event will fire for each new actor. When the player leaves the cell, the mobileDeactivated event is called.",
	eventData = {
		["reference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The reference that the mobile has been activated for.",
		},
	},
}