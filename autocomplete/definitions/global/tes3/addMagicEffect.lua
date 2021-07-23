return {
	type = "function",
	description = [[This function creates a new custom magic effect. The effect can be scripted through lua.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{
				name = "id",
				type = "tes3.effect",
				description = "Id of the new effect. Maps to tes3.effect.* added with tes3.claimSpellEffectId. If the effect of this id already exists, an error will be thrown."
			},
			{
				name = "name",
				type = "string",
				optional = true,
				default = "Unnamed Effect",
				description = "Name of the effect."
			},
			{
				name = "baseCost",
				type = "number",
				optional = true,
				default = 1.0,
				description = "Base magicka cost for the effect."
			},
			{
				name = "school",
				type = "tes3.magicSchool",
				optional = true,
				default = "tes3.magicSchool.alteration",
				description = "The magic school the new effect will be assigned to. Maps to tes3.magicSchool.* constants"
			},
			{
				name = "size",
				type = "number",
				optional = true,
				default = 1.0,
				-- description = ""
			},
			{
				name = "sizeCap",
				type = "number",
				optional = true,
				default = 1.0,
				-- description = ""
			},
			{
				name = "speed",
				type = "number",
				optional = true,
				default = 1.0,
				-- description = ""
			},
			{
				name = "description",
				type = "string",
				optional = true,
				default = "No description available.",
				description = "Description for the effect."
			},
			{
				name = "lighting",
				optional = true,
				type = "table",
				tableParams = {
					{
						name = "x",
						type = "number",
						default = 1.0,
						description = "Value of red color channel. In range of 0 - 1."
					},
					{
						name = "y",
						type = "number",
						default = 1.0,
						description = "Value of green color channel. In range of 0 - 1."
					},
					{
						name = "z",
						type = "number",
						default = 1.0,
						description = "Value of blue color channel. In range of 0 - 1."
					},
				},
			},
			{
				name = "icon",
				type = "string",
				optional = true,
				description = "Path to the effect icon. Must be a string no longer than 31 characters long."
			},
			{
				name = "particleTexture",
				type = "string",
				optional = true,
				description = "Path to the particle texture to use for the effect. Must be a string no longer than 31 characters long."
			},
			{
				name = "castSound",
				type = "string",
				optional = true,
				description = "Path to the sound which will be played on casting a spell with this effect. Must be a string no longer than 31 characters long."
			},
			{
				name = "boltSound",
				type = "string",
				optional = true,
				description = "Path to the sound which will be played when a spell with this effect is in flight. Must be a string no longer than 31 characters long."
			},
			{
				name = "hitSound",
				type = "string",
				optional = true,
				description = "Path to the sound which will be played when a spell with this effect hits something. Must be a string no longer than 31 characters long."
			},
			{
				name = "areaSound",
				type = "string",
				optional = true,
				description = "Must be a string no longer than 31 characters long."
			},
			{
				name = "castVFX",
				type = "tes3physicalObject",
				optional = true,
				-- description = ""
			},
			{
				name = "boltVFX",
				type = "tes3physicalObject",
				optional = true,
				-- description = ""
			},
			{
				name = "hitVFX",
				type = "tes3physicalObject",
				optional = true,
				-- description = ""
			},
			{
				name = "areaVFX",
				type = "tes3physicalObject",
				optional = true,
				-- description = ""
			},
			{
				name = "allowEnchanting",
				type = "boolean",
				optional = true,
				default = true,
				description = "Can this effect be used in a custom enchantment?"
			},
			{
				name = "allowSpellmaking",
				type = "boolean",
				optional = true,
				default = true,
				description = "Can this effect be used in a custom spell?"
			},
			{
				name = "appliesOnce",
				type = "boolean",
				optional = true,
				default = true,
				-- description = ""
			},
			{
				name = "canCastSelf",
				type = "boolean",
				optional = true,
				default = true,
				description = "Can this effect used with cast on self range?"
			},
			{
				name = "canCastTarget",
				type = "boolean",
				optional = true,
				default = true,
				description = "Can this effect be used with cast on target range?"
			},
			{
				name = "canCastTouch",
				type = "boolean",
				optional = true,
				default = true,
				description = "Can this effect be used with cast on touch range?"
			},
			{
				name = "casterLinked",
				type = "boolean",
				optional = true,
				default = true,
				-- description = ""
			},
			{
				name = "hasContinuousVFX",
				type = "boolean",
				optional = true,
				default = true,
				description = "Will the effect's VFX be played during its whole duration?"
			},
			{
				name = "hasNoDuration",
				type = "boolean",
				optional = true,
				default = true,
				-- description = ""
			},
			{
				name = "hasNoMagnitude",
				type = "boolean",
				optional = true,
				default = true,
				-- description = ""
			},
			{
				name = "illegalDaedra",
				type = "boolean",
				optional = true,
				default = true,
				description = "Is this effect illegal to use in public, because it summons Daedra? Note: this mechanic is not implemented in the game. Some mods might rely on this parameter."
			},
			{
				name = "isHarmful",
				type = "boolean",
				optional = true,
				default = true,
				description = "Is this effect considered harmful and casting it can be considered as an attack?"
			},
			{
				name = "nonRecastable",
				type = "boolean",
				optional = true,
				default = true,
				description = "Can this effect be recast while it already is in duration?"
			},
			{
				name = "targetsAttributes",
				type = "boolean",
				optional = true,
				default = true,
				-- description = ""
			},
			{
				name = "targetsSkills",
				type = "boolean",
				optional = true,
				default = true,
				-- description = ""
			},
			{
				name = "unreflectable",
				type = "boolean",
				optional = true,
				default = true,
				description = "Can the effect be reflected?"
			},
			{
				name = "usesNegativeLighting",
				type = "boolean",
				optional = true,
				default = true,
				-- description = ""
			},
			{
				name = "onTick",
				type = "function",
				optional = true,
				description = "A function which will be called on each tick of a spell containing this effect."
			},
			{
				name = "onCollision",
				type = "function",
				optional = true,
				description = "A function which will be called when a spell containing this spell effect collides with something."
			},
		},
	}},
	returns = "effect",
	valuetype = "tes3effect"
}
