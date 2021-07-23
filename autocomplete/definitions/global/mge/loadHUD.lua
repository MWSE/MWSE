return {
	type = "function",
	description = [[Loads a HUD element. This is a warpper for MGELoadHUD.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "hud", type = "string" },
			{ name = "texture", type = "string" },
			{ name = "enable", type = "boolean", optional = true },
		},
	}},
}