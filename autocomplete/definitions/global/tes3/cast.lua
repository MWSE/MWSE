return {
	type = "function",
	description = [[Casts a spell from a given reference to a target reference. Touch effects will hit the target at any range, while target effects will create a projectile. By default, the spell always casts successfully and does not consume magicka. By default, an actor casting will stop and perform its cast animation, but the 'instant' flag can start the cast instantly and allows more control over the spell for NPCs.

If the caster is the player, the target parameter is optional; without a target, the player's touch effects will only hit targets in front of them, and target effects will create a projectile in the direction the player is facing.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3reference|tes3mobileActor|string", description = "The caster reference." },
			{ name = "target", type = "tes3reference|tes3mobileActor|string", optional = true, description = "The target reference. Optional only if the caster is the player." },
			{ name = "spell", type = "tes3spell|string", description = "The spell the caster uses." },
			{ name = "instant", type = "boolean", optional = true, default = false, description = "If `true`, the spell is cast instantly. No animation is performed." },
			{ name = "alwaysSucceeds", type = "boolean", optional = true, default = true, description = "If `true`, the spell cannot fail and does not consume magicka. If `false`, it is cast using the actor's spell skill, and requires and takes enough magicka to cast. For NPCs, this only applies if `instant` is `true`." },
			{ name = "bypassResistances", type = "boolean", optional = true, default = false, description = "If `true`, the spell will bypass the target's resistances. For NPCs, this only applies if `instant` is `true`." },
		},
	}},
	returns = {
		{ name = "success", type = "boolean", description = "Returns `true` if the spell was cast successfully. Returns `false` if `instant` is `false` and the player is trying to cast the spell, while being unable to cast spells under normal circumstances." }
	},
	examples = {
		["imitateExplodeSpell"] = {
			title = "This is an example of how to use tes3.cast instead of mwscript.explodeSpell",
		},
		["triggerTrap"] = {
			title = "Applying the spell of a trapped door or container on an actor"
		},
	},
}
