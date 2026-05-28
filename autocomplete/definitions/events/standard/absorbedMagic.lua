return {
	type = "event",
	description =
	[[This event is triggered after magic absorption absorbs and cancels a magic effect, and just before magic absorption gives magicka to the target. It can control the amount of magicka restored.

This event occurs once per effect restored, so a multi-effect spell may trigger this multiple times. The default amount restored is equal to the casting cost of the entire spell, which is the vanilla behaviour.]],
	related = { "magicAbsorb", "spellResist", "spellResisted" },
	eventData = {
		["target"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The actor that absorbed the spell.",
		},
		["mobile"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile actor that absorbed the spell.",
		},
		["absorb"] = {
			type = "number",
			description = "The amount of magicka to restore to the actor.",
		},
		["source"] = {
			type = "tes3alchemy|tes3enchantment|tes3spell",
			readOnly = true,
			description = "The magic source.",
		},
		["sourceInstance"] = {
			type = "tes3magicSourceInstance",
			readOnly = true,
			description = "The unique instance of the magic source.",
		},
	},
	filter = "target",
}
