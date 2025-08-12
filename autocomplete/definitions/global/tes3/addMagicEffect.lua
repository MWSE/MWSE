return {
	type = "function",
	description = [[This function creates a new custom magic effect. The effect can be scripted through lua. This function should be used inside [`magicEffectsResolved`](https://mwse.github.io/MWSE/events/magicEffectsResolved/) event callback.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{
				name = "id",
				type = "tes3.effect|integer",
				description = "Id of the new effect. Maps to newly claimed `tes3.effect` constants with `tes3.claimSpellEffectId()`. If the effect of this id already exists, an error will be thrown."
			},
			{
				name = "name",
				type = "string",
				optional = true,
				default = "Unnamed Effect",
				description = "Name of the effect."
			},
			{
				name = "magnitudeType",
				type = "string",
				optional = true,
				description = "The suffix describing the magnitude, when its value is 1. By default, this resolves to the sPoint GMST."
			},
			{
				name = "magnitudeTypePlural",
				type = "string",
				optional = true,
				description = "The suffix describing the magnitude, when its value is not 1. By default, this resolves to the sPoints GMST."
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
				type = "tes3.magicSchool|integer",
				optional = true,
				default = "tes3.magicSchool.alteration",
				description = "The magic school the new effect will be assigned to. Maps to [`tes3.magicSchool`](https://mwse.github.io/MWSE/references/magic-schools/) constants."
			},
			{
				name = "size",
				type = "number",
				optional = true,
				default = 1.0,
				description = "Controls how much the visual effect scales with its magnitude."
			},
			{
				name = "sizeCap",
				type = "number",
				optional = true,
				default = 1.0,
				description = "The maximum possible size of the projectile."
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
				type = "tes3vector3|table",
				description = "Value of red, green, and blue values of the color for both particle lighting and enchantment wraps. In range of [0.0, 1.0].",
			},
			{
				name = "icon",
				type = "string",
				description = "Path to the effect icon. Must be a string no longer than 31 characters long. Use double \\ as path separator."
			},
			{
				name = "particleTexture",
				type = "string",
				description = "Path to the particle texture to use for the effect. Must be a string no longer than 31 characters long."
			},
			{
				name = "castSound",
				type = "string",
				description = "The sound ID which will be played on casting a spell with this effect. Must be a string no longer than 31 characters long. If not specified, the default sound for the spell school will be used."
			},
			{
				name = "boltSound",
				type = "string",
				description = "The sound ID which will be played when a spell with this effect is in flight. Must be a string no longer than 31 characters long. If not specified, the default sound for the spell school will be used."
			},
			{
				name = "hitSound",
				type = "string",
				description = "The sound ID which will be played when a spell with this effect hits something. Must be a string no longer than 31 characters long. If not specified, the default sound for the spell school will be used."
			},
			{
				name = "areaSound",
				type = "string",
				description = "The sound ID which will be played on area of effect impact. Must be a string no longer than 31 characters long. If not specified, the default sound for the spell school will be used."
			},
			{
				name = "castVFX",
				type = "tes3physicalObject|string",
				optional = true,
				description = "The visual played when a spell with this effect is cast."
			},
			{
				name = "boltVFX",
				type = "tes3physicalObject|string",
				optional = true,
				description = "The visual played when a spell with this effect is in flight."
			},
			{
				name = "hitVFX",
				type = "tes3physicalObject|string",
				optional = true,
				description = "The visual played when a spell with this effect hits something."
			},
			{
				name = "areaVFX",
				type = "tes3physicalObject|string",
				optional = true,
				description = "The visual played when a spell with this effect, with area of effect hits something."
			},
			{
				name = "allowEnchanting",
				type = "boolean",
				optional = true,
				default = true,
				description = "A flag which controls whether this effect can be used in a custom enchantment."
			},
			{
				name = "allowSpellmaking",
				type = "boolean",
				optional = true,
				default = true,
				description = "A flag which controls whether this effect can be used in a custom spell."
			},
			{
				name = "appliesOnce",
				type = "boolean",
				optional = true,
				default = true,
				description = "A flag which controls whether this effect applies once or is a ticking effect."
			},
			{
				name = "canCastSelf",
				type = "boolean",
				optional = true,
				default = true,
				description = "A flag which controls whether this effect can be used with cast on self range."
			},
			{
				name = "canCastTarget",
				type = "boolean",
				optional = true,
				default = true,
				description = "A flag which controls whether this effect can be used with cast on target range."
			},
			{
				name = "canCastTouch",
				type = "boolean",
				optional = true,
				default = true,
				description = "A flag which controls whether this effect can be used with cast on touch range."
			},
			{
				name = "casterLinked",
				type = "boolean",
				optional = true,
				default = true,
				description = [[Access to the base flag that determines if this effect must end if caster is dead, or not an NPC/creature. Not allowed in container or door trap spells. Note that this property is hidden in the Construction Set.]]
			},
			{
				name = "hasContinuousVFX",
				type = "boolean",
				optional = true,
				default = true,
				description = "A flag which controls whether the effect's visual is continuously played during the whole duration of the effect."
			},
			{
				name = "hasNoDuration",
				type = "boolean",
				optional = true,
				default = true,
				description = "A flag which controls whether this effect doesn't have duration."
			},
			{
				name = "hasNoMagnitude",
				type = "boolean",
				optional = true,
				default = true,
				description = "A flag which controls whether this effect doesn't have magnitude."
			},
			{
				name = "illegalDaedra",
				type = "boolean",
				optional = true,
				default = true,
				description = "A flag which controls whether this effect is illegal to use in public, because it summons Daedra. Note: this mechanic is not implemented in the game. Some mods might rely on this parameter."
			},
			{
				name = "isHarmful",
				type = "boolean",
				optional = true,
				default = true,
				description = "A flag which controls whether this effect is considered harmful and casting it can be considered as an attack."
			},
			{
				name = "nonRecastable",
				type = "boolean",
				optional = true,
				default = true,
				description = "A flag which controls whether this effect can be recast while it already is in duration."
			},
			{
				name = "targetsAttributes",
				type = "boolean",
				optional = true,
				default = true,
				description = "A flag which controls whether this effect targets a certain attribute or attributes."
			},
			{
				name = "targetsSkills",
				type = "boolean",
				optional = true,
				default = true,
				description = "A flag which controls whether this effect targets a certain skill or skills."
			},
			{
				name = "unreflectable",
				type = "boolean",
				optional = true,
				default = true,
				description = "A flag which controls whether this effect can be reflected."
			},
			{
				name = "usesNegativeLighting",
				type = "boolean",
				optional = true,
				default = true,
				description = "A flag which controls whether this effect uses negative lighting."
			},
			{
				name = "onTick",
				type = "fun(e: tes3magicEffectTickEventData)",
				optional = true,
				description = [[A function which will be called on each tick of a spell containing this effect. Note: `dt` (frame time) scaling is handled automatically. This function typically calls `e:trigger()` to run the effect through the normal spell event system.]],
			},
			{
				name = "onCollision",
				type = "fun(e: tes3magicEffectCollisionEventData)",
				optional = true,
				description = "A function which will be called when a spell containing this spell effect collides with something."
			},
		},
	}},
	examples = {
		["fireEffect"] = {
			title = "Fire Damage effect",
			description = "An implementation of the vanilla Fire Damage effect. Also, three spells are constructed with newly created magic effect, which are added to the player. You can test this in-game.",
		}
	},
	returns = {{ name = "effect", type = "tes3magicEffect"}},
}
