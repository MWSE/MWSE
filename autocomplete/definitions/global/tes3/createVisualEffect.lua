return {
	type = "function",
	description = [[Creates an arbitrary and automatically tracked visual effect. The visual effect can be an `object` (VFX objects can be found in the statics section of the constuction set), or a `magicEffectId`. You must specify one of a `reference`, `position`, or `avObject` to attach it to.

Most VFX assignments are persistent, and only expire when their lifespan ends, an associated reference is destroyed, or a given spell serial is retired.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3reference", optional = true, description = "If provided the VFX will be attached to this reference." },
			{ name = "position", type = "tes3vector3|number[]", optional = true, description = "If provided the VFX will be attached relative to a position, and not follow a reference." },
			{ name = "avObject", type = "niAVObject", optional = true, description = "If provided the VFX will be attached to this scene object." },

			{ name = "object", type = "tes3physicalObject|string", optional = true, description = "The physical object to use as the VFX. To use an enchantment-style VFX, supply the magicEffectId parameter instead." },
			{ name = "magicEffectId", type = "tes3.effect", optional = true, description = "The magic effect ID to use to create an enchantment-style VFX. This will use most of the same VFX logic, but cannot be applied to a position or specific niAVObject." },

			{ name = "serial", type = "number", optional = true, description = "An associated tes3magicSourceInstance serial. If a serial is assigned to the VFX, the effect expiring will also remove the VFX. This is not used when creating an enchantment-style VFX." },
			{ name = "repeatCount", type = "number", optional = true, description = "A repeat count for the VFX. If provided, the key timing for the associated effect will be used, multiplied by this value, to determine the total lifespan of the VFX. This is not used when creating an enchantment-style VFX." },
			{ name = "lifespan", type = "number", optional = true, description = "The desired lifespan for the VFX. If not provided, the VFX will never expire." },
			{ name = "scale", type = "number", optional = true, default = 1.0, description = "The scale used to resize the given VFX. The default value will match the size used by most magical effect logic. This is not used when creating an enchantment-style VFX." },
			{ name = "verticalOffset", type = "number", optional = true, default = 0.0, description = "This offset will be used to position it above its anchor reference. This is not used when creating an enchantment-style VFX." },
		},
	}},
	returns = {
		{ name = "vfx", type = "tes3vfx", description = "A handle to the VFX that was created. This can be passed to `tes3.removeVisualEffect` to remove it from the reference." }
	}
}
