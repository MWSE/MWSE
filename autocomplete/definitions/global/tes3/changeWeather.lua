return {
	type = "function",
	description = [[Changes the current weather, either with a transition period or immediately. It only affects the weather simulation system, independent of regional weather settings.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "id", type = "tes3.weather|integer", description = "Maps to [`tes3.weather`](https://mwse.github.io/MWSE/references/weather-types/) constants." },
			{ name = "immediate", type = "boolean", optional = true, description = "When true, the weather changes immediately. When false, a transition to the selected weather is started." },
		}
	}},
}