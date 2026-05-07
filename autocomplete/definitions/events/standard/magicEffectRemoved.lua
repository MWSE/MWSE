return {
	type = "event",
	description = [[This event triggers when a magic source removes an active instance for one of its effects.

For a magic source with multiple effects, this event triggers once for each effect instance that is removed. If the same source effect is active on multiple references, `magicEffectAdded`, `magicEffectActivated`, `magicEffectDeactivated`, and `magicEffectRemoved` can each trigger once for each affected reference.]],
	related = { "magicEffectActivated", "magicEffectAdded", "magicEffectDeactivated", "spellTick" },
	eventData = {
		["mobile"] = {
			type = "tes3mobileActor",
			readOnly = true,
			deprecated = true,
			description = "The mobile actor the magic effect is removed from. Use `target.mobile` instead.",
		},
		["reference"] = {
			type = "tes3reference",
			readOnly = true,
			deprecated = true,
			description = "The reference of the mobile actor the magic effect is removed from. Use `target` instead.",
		},
		["caster"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The caster of the magic source. Can be `nil`.",
		},
		["target"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The target of the magic effect instance that caused the source effect to be removed.",
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
			description = "The unique instance of the magic effect that caused the source effect to be removed.",
		},
		["state"] = {
			type = "tes3.spellState",
			readOnly = true,
			description = "The state of the magic effect instance when the event fired.",
		},
	},
	filter = "source",
	examples = {
		["showRemovedEffect"] = {
			title = "Show Removed Effect",
			description = "Show the magic effect and source names when an effect instance is removed on the player.",
		},
	}
}
