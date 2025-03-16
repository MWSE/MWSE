return {
	type = "event",
	description = [[This event is triggered after magic is reflected, and before the target of the magic is re-assigned to the caster. This event occurs once per reflected effect in a spell, so a multi-effect spell may trigger this multiple times.]],
	related = { "magicReflect", "absorbedMagic" },
	eventData = {
		["target"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The actor that reflected the spell.",
		},
		["mobile"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile actor that reflected the spell.",
		},
		["reflectEffect"] = {
			type = "tes3activeMagicEffect",
			readOnly = true,
			description = "The specific reflect effect being tested. This is a `tes3activeMagicEffect` instead of a more common magic instance. You can lookup the magic source instance or effect instance with `tes3activeMagicEffect` accessors.",
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