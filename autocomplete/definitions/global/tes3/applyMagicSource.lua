return {
	type = "function",
	description = [[]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3reference|tes3mobileActor|string" },
			{ name = "source", type = "tes3object", optional = true, description = "A magic source to apply." },
			{ name = "name", type = "string", optional = true, description = "If applying alchemy as a source, you can specifiy a name for the magic source." },
			{
				name = "effects",
				optional = true,
				description = "A table of custom effects to apply as a potion. Maximal number of effects is 8.",
				type = "table" ,
				tableParams = {
					{ name = "id", type = "boolean", optional = true, default = -1, description = "ID of the effect." },
					{ name = "skill", type = "tes3.skill.* constants", optional = true, default = -1, description = "If effect parameter specified is: Absorb, Damage, Drain, Fortify or Restore Skill, a skill should be provided. This also applies to any custom spell effect which operates on a certain skill." },
					{ name = "attribute", type = "tes3.attribute.* constants", optional = true, default = -1, description = "If effect parameter specified is: Absorb, Damage, Drain, Fortify or Restore Attribute, an attribute should be provided. This also applies to any custom spell effect which operates on a certain attribute." },
					{ name = "range", type = "tes3.effectRange.* constants", optional = true, default = "tes3.effectRange.self", description = "The range of the effect. This maps to tes3.effectRange.* constants." },
					{ name = "radius", type = "number", optional = true, default = 0, description = "The radius of the effect." },
					{ name = "duration", type = "number", optional = true, default = 0, description = "Number of seconds the effect is going to be active." },
					{ name = "min", type = "number", optional = true, default = 0, description = "The minimal magintude of the effect per tick." },
					{ name = "max", type = "number", optional = true, default = 0, description = "The maximal magnitude of the effect per tick." },
				}
			},
			{ name = "createCopy", type = "boolean", optional = true, default = false, description = "This parameter controls whether the function will return the original magic source or a copy of the magic source. This parameter is only used if source is alchemy." },
			{ name = "fromStack", type = "tes3equipmentStack", optional = true, description = "The piece of equipment this magic source is coming from. The fromStack has to be an already equipped item from tes3actor.equipment. This will probably change in the future." },
			{ name = "castChance", type = "number", optional = true, default = "This parameter allows overriding the casting chance of the magic source." },
			{ name = "target", type = "tes3reference|tes3mobileActor|string", optional = true, description = "The target of the magic." },
			{ name = "bypassResistances", type = "boolean", optional = true, default = false, description = "Is this effect going to bypass magic resistance?" },
		},
	}},
	returns = "instance",
	valuetype = "tes3magicSourceInstance",
}
