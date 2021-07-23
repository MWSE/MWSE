return {
	type = "function",
	description = [[Locates a root dialogue topic that can then be filtered down for a specific actor to return a specific dialogue info. For example, a type of ``tes3.dialogueType.greeting`` and a page of ``tes3.dialoguePage.greeting.greeting0`` will return the "Greeting 0" topic.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "type", type = "number", description = "The type of dialogue to look for. Uses tes3.dialogueType.* constants." },
			{ name = "page", type = "number", description = "The page of dialogue to fetch. Uses tes3.dialoguePage.* constants." },
		},
	}},
	returns = {{ name = "dialogue", type = "tes3dialogue" }},
}
