return {
	type = "method",
	description = [[Gets the precipitation blend state for the given weather type.]],
	arguments = {
		{ name = "weather", type = "tes3.weather|number", description = "The weather index to use the precipitation of." },
		{ name = "relevance", type = "number", description = "The scalar for the result." },
	},
	returns = {
		{ name = "blend", type = "number", description = "The combined relevence for the precipitation type." },
	},
}
