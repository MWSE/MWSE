return {
	description = "The damaged event triggers after an actor has been damaged.",
	eventData = {
		damage = {
			type = "number",
			readOnly = true,
			description = "The amount of damage done.",
		},
		mobile = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile actor that took damage.",
		},
		reference = {
			type = "tes3reference",
			readOnly = true,
			description = "mobile’s associated reference.",
		},
        	attacker = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile actor dealing the damage. Can be nil.",
		},
		attackerReference = {
			type = "tes3reference",
			readOnly = true,
			description = "attacker mobile's associated reference. Can be nil.",
		},
		projectile = {
			type = "tes3mobileProjectile",
			readOnly = true,
			description = "Projectile that dealt the damage. Can be nil.",
		},
		activeMagicEffect = {
			type = "tes3magicEffect",
			readOnly = true,
			description = "tes3magicEffect which caused damage. Can be nil.",
		},
		magicSourceInstance = {
			type = "tes3magicSourceInstance",
			readonly = true ,
			description = "tes3magicSourceInstance of a spell that caused damage. Can be nil.",
		},
		source = {
			type ="damageSourceType",
			readOnly = true,
			description = "The origin of damage. Values of this variable can be: \"script\", \"fall\", \"suffocation\", \"attack\", \"magic\", \"shield\" or nil.",
		},
		killingBlow = {
			type ="boolean",
			readOnly = true,
			description = "If true, the damage killed the target.",
		},
	},
	examples = {
		["killingBlow"] = {
			title = "Notify the player that their arrow/bolt killed their opponent"
		},
	},
}
