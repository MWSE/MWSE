return {
	type = "event",
	description = [[This event is invoked whenever an actor starts an attack with their fists or a weapon, or a creature makes any attack. More precisely, it is when the actor raises a melee weapon or draws an arrow. There is not necessarily a target in range, or any target at all for the player.

Lockpicks and probes do not invoke this event.]],
	related = { "attack", "attackHit" },
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
			type = "tes3.physicalAttackType",
			description = "A number from the [`tes3.physicalAttackType`](https://mwse.github.io/MWSE/references/physical-attack-types/) enumeration identifying the physical attack type. Can be `tes3.physicalAttackType.slash`, `.chop`, `.thrust`, `.projectile`, `.creature1`, `.creature2`, or `.creature3`. May be changed to change the attack's type if the original attack was slash, chop, or thrust.",
		},
		["attackSpeed"] = {
			type = "number",
			description = "The speed multiplier of the attack animation, normally equal to the weapon speed (1.0 for no equipped weapon). May be changed to change the current attack's speed. See also [tes3actorAnimationController](https://mwse.github.io/MWSE/types/tes3actorAnimationController/) to change animation speed during the attack.",
		},
	},
	blockable = true,
	examples = {
		["directionChange"] = {
			title = "Changing axe attack direction",
			description = [[]],
		},
		["blockAttack"] = {
			title = "Blocking an attack from starting",
		},
	},
	filter = "reference",
}