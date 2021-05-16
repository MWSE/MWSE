return {
	description = "The damage event triggers before an actor is damaged. The damage value can be modified, or the damage can be prevented completely by blocking the event.",
	eventData = {
		damage = {
			type = "number",
			description = "The amount of damage done.",
		},
		mobile = {
			type = "tes3mobileActor",
			readonly = true,
			description = "The mobile actor that is taking damage.",
		},
		reference = {
			type = "tes3reference",
			readonly = true,
			description = "mobile’s associated reference.",
		},
		attacker = {
			type = "tes3mobileActor",
			readonly = true,
			description = "The mobile actor dealing the damage. Can be nil.",
		},
		attackerReference = {
			type = "tes3reference",
			readonly = true,
			description = "attacker mobile's associated reference. Can be nil.",
		},
		projectile = {
	        type = "tes3mobileProjectile",
			readonly = true,
			description = "Projectile that dealt the damage. Can be nil.",
	    },
		activeMagicEffect = {
			type = "tes3magicEffect",
			readonly = true,
			description = "tes3magicEffect which caused damage. Can be nil.",
		},
		magicSourceInstance = {
			type = "tes3magicSourceInstance",
			readonly = true ,
			description = "tes3magicSourceInstance of a spell that caused damage. Can be nil.",
		},
		source = {
			type ="damageSourceType",
			readonly = true,
			description = "The origin of damage. Values of this variable can be: \"script\", \"fall\", \"suffocation\", \"attack\", \"magic\", \"shield\" or nil.",
		},
	},
}
