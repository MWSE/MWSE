return {
	type = "event",
	description = "The spellProjectileHitActor event fires when a spell projectile collides with an actor.",
	related = { "spellProjectileHitActor", "spellProjectileHitObject", "spellProjectileHitTerrain", "spellProjectileHitWater" },
	eventData = {
		["mobile"] = {
			type = "tes3mobileSpellProjectile",
			readOnly = true,
			description = "The spell projectile that is expiring.",
		},
		["target"] = {
			type = "tes3reference",
			readOnly = true,
			description = "Reference to the actor that was hit.",
		},
		["collisionPoint"] = {
			type = "tes3vector3",
			readOnly = true,
			description = "The collision point of the spell projectile.",
		},
		["position"] = {
			type = "tes3vector3",
			readOnly = true,
			description = "The position of the spell projectile at collision.",
		},
		["velocity"] = {
			type = "tes3vector3",
			readOnly = true,
			description = "The velocity of the spell projectile at collision.",
		},
		["firingReference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "Reference to the actor that fired the projectile.",
		},
	},
}
