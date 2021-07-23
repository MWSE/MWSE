return {
	type = "function",
	description = [[Advances the game time. Can be used to simulate player resting.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "hours", type = "number", description = "How many hours to progress." },
			{ name = "resting", type = "boolean", optional = true, default = false, description = "Should advancing time count as resting? If set to true invokes usual sleeping mechanics: health, fatigue and magicka restoration, and possible rest interruption. The length of the rest will be equal to hours parameter, rounded down to nearest natural number." },
			{ name = "updateEnvironment", type = "boolean", optional = true, default = true, description = "Controls if the weather system is updated for each hour passed." },
		},
	}},
	returns = "hoursPassed",
	valuetype = "number",
}
