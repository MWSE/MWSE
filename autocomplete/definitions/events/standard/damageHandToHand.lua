return {
	description = "The damageHandToHand event triggers before an actor takes fatigue damage from hand-to-hand combat. It does not trigger on health damage, but the 'damage' event will. The fatigueDamage value can be modified, or can be prevented completely by blocking the event. The player as attacker will gain hand-to-hand experience only if fatigueDamage is greater than zero.",
	eventData = {
		fatigueDamage = {
			type = "number",
			description = "The amount of fatigue damage done.",
		},
		mobile = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile actor that is taking fatigue damage.",
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
	},
}
