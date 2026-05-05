return {
	type = "event",
	description = [[This event triggers when an individual magic effect becomes inactive on its target.

For a magic source with multiple effects, this event triggers once for each effect as that effect becomes inactive on a target. If a source effect affected multiple references, this event can trigger for each reference before `magicEffectEnded` triggers for the source effect.]],
	related = { "magicEffectActivated", "magicEffectBegan", "magicEffectEnded", "magicEffectRemoved", "spellTick" },
	eventData = {
		["caster"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The caster of the magic source. Can be `nil`.",
		},
		["target"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The target of the magic effect instance that became inactive.",
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
			description = "The unique instance of the magic effect that became inactive.",
		},
		["state"] = {
			type = "tes3.spellState",
			readOnly = true,
			description = "The state of the magic effect instance when the event fired.",
		},
	},
	filter = "source",
	examples = {
		["showDeactivatedEffect"] = {
			title = "Show Deactivated Effect",
			description = "Show the magic effect and source names when an effect becomes inactive on the player.",
		},
	}
}
