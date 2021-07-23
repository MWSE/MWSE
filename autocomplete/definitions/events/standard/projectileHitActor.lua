return {
	description = "The projectileHitActor event fires when a projectile collides with an actor.",
	eventData = {
		["mobile"] = {
			type = "tes3mobileProjectile",
			readOnly = true,
			description = "The mobile projectile that is expiring.",
		},
		["target"] = {
			type = "tes3reference",
			readOnly = true,
			description = "Reference to the actor that was hit.",
		},
		["firingReference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "Reference to the actor that fired the projectile.",
		},
		["firingWeapon"] = {
			type = "tes3weapon",
			readOnly = true,
			description = "The weapon that fired the projectile.",
		},
	},
}