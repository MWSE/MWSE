return {
	type = "function",
	description = [[Modifies the effective disposition of an NPC, and updates the dialogue UI if visible. The change is clamped so that effective disposition remains within the range 0-100. The change can be either permanent or temporary (limited to a dialogue session).]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3mobileActor|tes3reference|string" },
			{ name = "value", type = "integer", description = "The change in disposition." },
			{ name = "temporary", type = "boolean", optional = true, default = false, description = "When true, the disposition change will only temporarily modify disposition while the dialogue window is open. Temporary changes have no effect outside dialogue." },
		},
	}},
}