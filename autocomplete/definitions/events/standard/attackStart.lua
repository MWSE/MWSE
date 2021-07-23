return {
	description = "This event is invoked whenever an actor starts an attack with their fists or a weapon, or a creature makes any attack. It is when the actor raises a melee weapon or draws an arrow. There is not necessarily a target in range, or any target at all for the player.",
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
		["attackType"] = {
			type = "number",
			description = "A number from the tes3.physicalAttackType enumeration identifying the physical attack type. Can be tes3.physicalAttackType.slash, .chop, .thrust, .projectile, .creature1, .creature2, or .creature3. May be changed to change the attack's type if the original attack was slash, chop, or thrust.",
		},
		["attackSpeed"] = {
			type = "number",
			description = "The speed multiplier of the attack animation, normally equal to the weapon speed (1.0 for no equipped weapon). May be changed to change the current attack's speed. See also tes3.actorAnimationController to change animation speed during the attack.",
		},
	},
	filter = "reference",
}