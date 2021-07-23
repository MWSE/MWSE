return {
	description = "The uiSpellTooltip event triggers when a new tooltip is displayed for a spell. The tooltip will be already be built.",
	eventData = {
		["tooltip"] = {
			type = "tes3uiElement",
			readOnly = true,
			description = "The newly created tooltip element. Due to timeouts and target changes, it may be destroyed at any time.",
		},
		["spell"] = {
			type = "tes3spell",
			readOnly = true,
			description = "The spell being examined.",
		},
	},
}