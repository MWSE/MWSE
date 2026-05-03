return {
	type = "function",
	description = [[This function creates a new custom weather. The weather can be scripted through lua. This function should be used inside the [`initialized`](https://mwse.github.io/MWSE/events/initialized/) event callback.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{
				name = "id",
				type = "tes3.weather|integer",
				description = "The unique id of the weather. Must be greater than 10 and less than 254, and must not already be used by another custom weather."
			},
			{
				name = "key",
				type = "string",
				description = "A unique string key for the weather. This is the key added to the `tes3.weather` table, such that `tes3.weather[key] = id`."
			},
			{
				name = "name",
				type = "string",
				description = "The name of the custom weather."
			},
			{
				name = "ambientDayColor",
				type = "tes3vector3|table",
				description = "Ambient light color during the day."
			},
			{
				name = "ambientNightColor",
				type = "tes3vector3|table",
				description = "Ambient light color during the night."
			},
			{
				name = "ambientSunriseColor",
				type = "tes3vector3|table",
				description = "Ambient light color during sunrise."
			},
			{
				name = "ambientSunsetColor",
				type = "tes3vector3|table",
				description = "Ambient light color during sunset."
			},
			{
				name = "cloudsMaxPercent",
				type = "number",
				optional = true,
				default = 1.0,
				description = "The maximum percentage of cloud coverage. Requires a value between 0.0 and 1.0."
			},
			{
				name = "cloudsSpeed",
				type = "number",
				optional = true,
				default = 1.25,
				description = "The speed that clouds move across the sky."
			},
			{
				name = "fogDayColor",
				type = "tes3vector3|table",
				description = "Fog color during the day."
			},
			{
				name = "fogNightColor",
				type = "tes3vector3|table",
				description = "Fog color during the night."
			},
			{
				name = "fogSunriseColor",
				type = "tes3vector3|table",
				description = "Fog color during sunrise."
			},
			{
				name = "fogSunsetColor",
				type = "tes3vector3|table",
				description = "Fog color during sunset."
			},
			{
				name = "glareView",
				type = "number",
				optional = true,
				default = 1.0,
				description = "Controls how strongly sunlight glares into the camera."
			},
			{
				name = "landFogDayDepth",
				type = "number",
				optional = true,
				default = 0.69,
				description = "The land fog depth during the day."
			},
			{
				name = "landFogNightDepth",
				type = "number",
				optional = true,
				default = 0.69,
				description = "The land fog depth during the night."
			},
			{
				name = "skyDayColor",
				type = "tes3vector3|table",
				description = "Sky color during the day."
			},
			{
				name = "skyNightColor",
				type = "tes3vector3|table",
				description = "Sky color during the night."
			},
			{
				name = "skySunriseColor",
				type = "tes3vector3|table",
				description = "Sky color during sunrise."
			},
			{
				name = "skySunsetColor",
				type = "tes3vector3|table",
				description = "Sky color during sunset."
			},
			{
				name = "sunDayColor",
				type = "tes3vector3|table",
				description = "Sun color during the day."
			},
			{
				name = "sundiscSunsetColor",
				type = "tes3vector3|table",
				description = "Sun disc color during sunset."
			},
			{
				name = "sunNightColor",
				type = "tes3vector3|table",
				description = "Sun color during the night."
			},
			{
				name = "sunSunriseColor",
				type = "tes3vector3|table",
				description = "Sun color during sunrise."
			},
			{
				name = "sunSunsetColor",
				type = "tes3vector3|table",
				description = "Sun color during sunset."
			},
			{
				name = "windSpeed",
				type = "number",
				optional = true,
				default = 0.1,
				description = "The speed of the wind."
			},
			{
				name = "cloudTexture",
				type = "string",
				optional = true,
				description = "Path to the cloud texture used by the weather."
			},
			{
				name = "ambientLoopSoundId",
				type = "string",
				optional = true,
				description = "The sound ID for the ambient looping sound."
			},
			{
				name = "overrideId",
				type = "tes3.weather|integer",
				optional = true,
				description = "An override weather id used by the weather system. If provided, dialogue and script checks against the provided ID will return true for this weather as well. This can be used for backwards compatibility with vanilla weathers."
			},
			{
				name = "rainThreshold",
				type = "number",
				optional = true,
				description = "The transition threshold to be considered raining."
			},
			{
				name = "stormThreshold",
				type = "number",
				optional = true,
				description = "The transition threshold to be considered storming."
			},
			{
				name = "snowThreshold",
				type = "number",
				optional = true,
				description = "The transition threshold to be considered snowing."
			},
			{
				name = "raindropsMax",
				type = "number",
				optional = true,
				description = "The maximum amount of raindrop particles."
			},
			{
				name = "snowflakesMax",
				type = "number",
				optional = true,
				description = "The maximum amount of snowflake particles."
			},
			{
				name = "windJitterScalar",
				type = "number",
				default = 1.0,
				description = "The significance that wind speed has on randomized cloud movement."
			},
			{
				name = "supportsParticleLerp",
				type = "boolean",
				optional = true,
				default = false,
				description = "If true, particle lerping is enabled for this weather."
			},
			{
				name = "supportsAshCloud",
				type = "boolean",
				optional = true,
				default = false,
				description = "If true, this weather can support ash clouds."
			},
			{
				name = "supportsBlightCloud",
				type = "boolean",
				optional = true,
				default = false,
				description = "If true, this weather can support blight clouds."
			},
			{
				name = "supportsBlizzard",
				type = "boolean",
				optional = true,
				default = false,
				description = "If true, this weather can support blizzard effects."
			},
			{
				name = "simulate",
				type = "fun(e: tes3weatherSimulateEventData)",
				optional = true,
				description = "A function called every frame while the weather is active."
			},
			{
				name = "transition",
				type = "fun(e: tes3weatherTransitionEventData)",
				optional = true,
				description = "A function called when the weather transitions in. The ambient loop is automatically given a new "
			},
			{
				name = "unload",
				type = "fun(e: tes3weatherUnloadEventData)",
				optional = true,
				description = "A function called when the weather is unloaded. The soundAmbientLoop is handled automatically."
			},
		},
	}},
	examples = {
		-- TODO
	},
	returns = {{ name = "weather", type = "tes3weatherCustom"}},
}
