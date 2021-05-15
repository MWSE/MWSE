return {
	description = "The damaged event triggers after an actor has been damaged.",
	eventData = {
		damage = {
			type = "number",
			readonly = true,
			description = "The amount of damage done.",
		},
		mobile = {
			type = "tes3mobileActor",
			readonly = true,
			description = "The mobile actor that took damage.",
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
			type = "tes3projectile", -- check this one
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
            		type ="damageSourceType",   -- Maybe change this
            		readonly = true,
            		description = "The origin of damage. Values of this variable can be: \"script\", \"fall\", \"suffocation\", \"attack\", \"magic\" or \"shield\".",
        	},
        	checkForKnockdown = {   --Not sure what this one does
            		type ="bool",
            		readonly = true,
            		description = ""
        	},
	},
}
