return {
	type = "value",
	description = [[An array-style table of the tes3effect data on the object.]],
	readOnly = true,
	valuetype = "table",
	-- tableParams here doesn't do anything. This may help someone in the future.
	--[[
	tableParams = {
		{ name = "id", type = "number", description = "The base tes3magicEffect ID that the effect uses." },
		{ name = "skill", type = "number", description = "The skill associated with this effect, or -1 if no skill is used. Maps to tes3.skill.* constants." },
		{ name = "attribute", type = "number", description = "The attribute associated with this effect, or -1 if no attribute is used. Maps to tes3.attribute.* constants." },
		{ name = "range", type = "number", description = "Determines if the effect is self, touch, or target ranged. Maps to tes3.effectRange.* constants." },
		{ name = "radius", type = "number", description = "The radius of the effect." },
		{ name = "duration", type = "number", description = "How long the effect lasts." },
		{ name = "min", type = "number", description = "The minimum magnitude of the effect." },
		{ name = "max", type = "number", description = "The maximum magnitude of the effect." },
	},
  ]]
}
