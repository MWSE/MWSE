return {
	type = "event",
	description = [[This event triggers when a magic source removes the last active instance for one of its effects.

For a magic source with multiple effects, this event triggers once for each effect that ends. If the same source effect is active on multiple references, `magicEffectDeactivated` can trigger as each reference stops being affected, but `magicEffectEnded` only triggers when the final stored instance of that source effect is retired.]],
	related = { "magicEffectActivated", "magicEffectBegan", "magicEffectDeactivated", "magicEffectRemoved", "spellTick" },
	eventData = {
		["caster"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The caster of the magic source. Can be `nil`.",
		},
		["target"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The target of the magic effect instance that caused the source effect to end.",
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
			description = "The unique instance of the magic effect that caused the source effect to end.",
		},
		["state"] = {
			type = "tes3.spellState",
			readOnly = true,
			description = "The state of the magic effect instance when the event fired.",
		},
	},
	filter = "source",
	examples = {
		["showEndedEffect"] = {
			title = "Show Ended Effect",
			description = "Show the magic effect and source names when a source effect ends on the player.",
		},
	}
}
