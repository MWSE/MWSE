return {
	type = "event",
	description = [[This event is triggered before a magic reflection check. It allows changing the reflection chance. Note that some magic effects that the player casts are unreflectable (usually non-damage effects, see magic effect flags) and do not trigger this event.
	
Each reflect effect active on a target will roll separately; the reflect chances are independent. This event also occurs once per effect in a spell, so a multi-effect spell may trigger this multiple times.]],
	related = { "magicReflected", "absorbedMagic" },
	eventData = {
		["target"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The actor that may reflect the spell.",
		},
		["mobile"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile actor that may reflect the spell.",
		},
		["reflectEffect"] = {
			type = "tes3activeMagicEffect",
			readOnly = true,
			description = "The specific reflect effect being tested. This is a `tes3activeMagicEffect` instead of a more common magic instance. You can lookup the magic source instance or effect instance with `tes3activeMagicEffect` accessors.",
		},
		["reflectChance"] = {
			type = "number",
			description = "The % chance that the magic is reflected. May be modified.",
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