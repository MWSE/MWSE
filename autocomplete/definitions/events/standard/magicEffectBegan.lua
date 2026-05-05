return {
	type = "event",
	description = [[This event triggers when a magic source stores the first active instance for one of its effects.

For a magic source with multiple effects, this event triggers once for each effect that begins. For example, a spell with fire damage and frost damage effects triggers once when the fire damage effect begins and once when the frost damage effect begins. If additional references are affected by the same source effect later, `magicEffectActivated` triggers for those references, but `magicEffectBegan` does not trigger again until a new magic source effect instance is created.]],
	related = { "magicEffectActivated", "magicEffectDeactivated", "magicEffectEnded", "magicEffectRemoved", "spellTick" },
	eventData = {
		["caster"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The caster of the magic source. Can be `nil`.",
		},
		["target"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The target of the magic effect instance that caused the source effect to begin.",
		},
		["source"] = {
			type = "tes3alchemy|tes3enchantment|tes3spell",
			readOnly = true,
			description = "The magic source that contains the effect.",
		},
		["sourceInstance"] = {
			type = "tes3magicSourceInstance",
			readOnly = true,
			description = "The unique instance of the magic source that contains the effect.",
		},
		["effect"] = {
			type = "tes3effect",
			readOnly = true,
			description = "The specific effect that triggered the event. This is equal to `e.source.effects[e.effectIndex]`. Can be `nil`.",
		},
		["effectIndex"] = {
			type = "integer",
			readOnly = true,
			description = "The index of the effect in the magic source's effects list.",
		},
		["effectInstance"] = {
			type = "tes3magicEffectInstance",
			readOnly = true,
			description = "The unique instance of the magic effect that caused the source effect to begin.",
		},
		["state"] = {
			type = "tes3.spellState",
			readOnly = true,
			description = "The state of the magic effect instance when the event fired.",
		},
	},
	filter = "source",
	examples = {
		["showBeganEffect"] = {
			title = "Show Began Effect",
			description = "Show the magic effect and source names when a source effect begins on the player.",
		},
	}
}
