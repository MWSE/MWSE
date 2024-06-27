return {
	type = "event",
	description = "This event is raised just before a touch spell completes casting, to set up the hit detection cone for spell targets. See `calcHitDetectionCone` for the melee equivalent.",
	related = { "calcHitDetectionCone" },
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
		["reach"] = {
			type = "number",
			description = "The touch spell search range in world units.",
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
