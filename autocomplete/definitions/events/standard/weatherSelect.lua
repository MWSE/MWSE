return {
	type = "event",
	description = [[Occurs when a region picks a new weather during weather cycling or weather-related script operations. It exposes the region's base weather chances as a mutable dictionary keyed by raw weather index. Lua may overwrite vanilla weights, remove them by setting them to `0`, or add positive weights for custom weather IDs. The resulting weights do not need to total 100.]],
	related = { "weatherCycled", "weatherTransitionStarted", "weatherChangedImmediate" },
	eventData = {
		region = {
			type = "tes3region",
			readOnly = true,
			description = "The region selecting a new weather.",
		},
		chances = {
			type = "table",
			valuetype = "table<integer, number>",
			description = "A mutable dictionary of weather weights keyed by weather index. Only entries with positive numeric weights and a loaded weather object are considered during selection.",
		},
	},
	filter = "region",
	examples = {
		["BiasRegionWeather"] = {
			title = "Bias a region toward a custom weather at runtime",
		},
	},
}
