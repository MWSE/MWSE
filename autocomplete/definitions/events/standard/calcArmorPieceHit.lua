return {
	type = "event",
	description = [[This event is raised just after determining which armor piece, if any, was hit with an attack. The slots can be modified, to draw focus onto specific armor slots.

The following table is used by default to determine what armor piece is hit:

Roll (1-100) | Slot            | Fallback
------------ | --------------- | ---------------
1-29         | Cuirass         | *None*
30-34        | Shield          | Cuirass
35-39        | Shield          | Left Pauldron
40-49        | Left Pauldron   | *None*
50-59        | Right Pauldron  | *None*
60-69        | Greaves         | *None*
70-79        | Helmet          | *None*
80-89        | Boots           | *None*
90-94        | Right Gauntlet  | Right Bracer
95-100       | Left Gauntlet   | Left Bracer]],
	eventData = {
		["mobile"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile who is being hit.",
		},
		["reference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "A shortcut to the mobile's reference.",
		},
		["attackerMobile"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile who is doing the hitting.",
		},
		["attacker"] = {
			type = "tes3reference",
			readOnly = true,
			description = "A shortcut to the attacker mobile's reference.",
		},
		["projectile"] = {
			type = "tes3mobileProjectile|nil",
			readOnly = true,
			description = "The projectile used that is hitting the given reference. This is nil for all melee combat.",
		},
		["slot"] = {
			type = "tes3.armorSlot",
			description = "The primary slot that is targeted.",
		},
		["fallback"] = {
			type = "tes3.armorSlot",
			description = "A secondary slot to check for equipment on, if no armor is found using the primary slot.",
		},
	},
	filter = "reference.baseObject",
}
