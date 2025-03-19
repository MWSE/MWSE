return {
	type = "event",
	description = "This event is raised before a melee hit attempt, to set up the hit detection cone of a attack swing. It is also raised a second time when the attack animation reaches its hit point, to check if the target is still within reach. See `calcTouchSpellCone` for the touch spell equivalent.",
	related = { "calcTouchSpellCone" },
	eventData = {
		["attacker"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The reference of the attacker.",
		},
		["attackerMobile"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile which is making the attack.",
		},
		["target"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The reference of the target. Only available at the second stage (weapon strike).",
		},
		["targetMobile"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile which is targetted. Only available at the second stage (weapon strike).",
		},
		["reach"] = {
			type = "number",
			description = "The weapon's reach multiplier, typically in the range 0.5 - 2.0. May be adjusted.",
		},
		["angleXY"] = {
			type = "number",
			description = "The maximum allowable angle offset (in degrees) in the horizontal direction that will hit. This is related to game setting fCombatAngleXY but using different units. May be adjusted. The highest effective angle is 90 degrees, and larger angles will behave like 90 degrees.",
		},
		["angleZ"] = {
			type = "number",
			description = "The maximum allowable angle offset (in degrees) in the vertical direction that will hit. This is related to game setting fCombatAngleZ but using different units. May be adjusted. The highest effective angle is 90 degrees, and larger angles will behave like 90 degrees.",
		},
	},
	filter = "attacker.baseObject",
}
