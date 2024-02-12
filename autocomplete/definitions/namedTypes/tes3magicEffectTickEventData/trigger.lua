return {
	type = "method",
	description = [[Allows the effect to run through the normal spell event system.]],
	arguments = {{
		name = "params",
		type = "table",
		optional = true,
		tableParams = {
			{
				name = "negateOnExpiry",
				type = "boolean",
				optional = true,
				default = true,
				description = "If this flag is `true`, the effect will be negated on expiry."
			},
			{
				name = "isUncapped",
				type = "boolean",
				optional = true
			},
			{
				name = "attribute",
				type = "tes3.effectAttribute",
				optional = true,
				default = "tes3.effectAttribute.nonResistable",
				description = "The attribute used in resistance calculations agains this effect. Maps to values in [`tes3.effectAttribute`](https://mwse.github.io/MWSE/references/effect-attributes/) table."
			},
			{
				name = "type",
				type = "tes3.effectEventType",
				optional = true,
				default = "tes3.effectEventType.boolean",
				description = "This flag controls how the effect behaves. For example, `tes3.effectEventType.modStatistic` will make the effect work as calling `tes3.modStatistic`. Maps to values in [`tes3.effectEventType`](https://mwse.github.io/MWSE/references/effect-event-types/) table."
			},
			{
				name = "value",
				type = "boolean|integer|number|tes3statistic",
				optional = true,
				default = 0,
				description = "The variable this effect changes. This can be a local variable in a script or a tes3statistic property on a `tes3mobileActor`. The type of the passed variable must match the type of the `type` parameter."
			},
			{
				name = "resistanceCheck",
				type = "fun(e: tes3magicEffectResistenceCheckEventData): boolean?",
				optional = true,
				description = [[The function passed as `resistanceCheck` will be used on any of the game's spell resistance checks. Returning `true` from this function will set your effect to expired, and depending on your trigger code may stop processing.

For example, the only effect in vanilla Morrowind that implements this function is Water Walking. It disallows using a spell with Water Walking when the player is deep underwater, by setting it as expired.]]
			},
		}
	}},
	returns = {
		{ name = "eventResult", type = "boolean" },
		{
			name = "modifiedValue",
			type = "boolean|integer|number|tes3statistic",
			description = "The passed `value`, scaled by resistance. The returned type depends on the passed `type` parameter."
		}
	}
}
