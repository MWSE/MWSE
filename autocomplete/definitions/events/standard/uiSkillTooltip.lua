return {
	type = "event",
	description = [[The uiSkillTooltip event triggers when a new tooltip is displayed for a skill. The tooltip will be already be built.

Note that tooltips may have different width depending on which menu they were created in, between 230 and 432 pixels by default.
	]],
	related = { "uiObjectTooltip", "uiSpellTooltip" },
	eventData = {
		["tooltip"] = {
			type = "tes3uiElement",
			readOnly = true,
			description = "The newly created tooltip element. Due to timeouts and target changes, it may be destroyed at any time.",
		},
		["creator"] = {
			type = "tes3uiElement?",
			readOnly = true,
			description = "The UI element, if applicable, that created this tooltip.",
		},
		["skill"] = {
			type = "tes3.skill",
			readOnly = true,
			description = "The id of the skill being examined.",
		},
		["type"] = {
			type = "integer",
			readOnly = true,
			description = "1 where the tooltip is being created by MenuStat, and includes progress to the next skill level up, and 0 for every other instance including tes3ui.createTooltipMenu().",
		},
	},
}