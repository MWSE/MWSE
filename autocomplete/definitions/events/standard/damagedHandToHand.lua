return {
	type = "event",
	description = "The damagedHandToHand event triggers after an actor takes fatigue damage from hand-to-hand combat. It does not trigger on health damage, but the `damaged` event will.",
	related = { "damage", "damaged", "damageHandToHand", "damagedHandToHand" },
	eventData = {
		fatigueDamage = {
			type = "number",
			readOnly = true,
			description = "The amount of fatigue damage done.",
		},
		mobile = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile actor that took fatigue damage.",
		},
		reference = {
			type = "tes3reference",
			readOnly = true,
			description = "The mobile’s associated reference.",
		},
		attacker = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile actor dealing the damage. Can be nil.",
		},
		attackerReference = {
			type = "tes3reference",
			readOnly = true,
			description = "The attacker mobile's associated reference. Can be nil.",
		},
		source = {
			type ="tes3.damageSource",
			readOnly = true,
			description = "The origin of the damage. May be `tes3.damageSource.attack` or `tes3.damageSource.script`.",
		},
	},
}
