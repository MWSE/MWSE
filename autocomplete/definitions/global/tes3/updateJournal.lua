return {
	type = "function",
	description = [[Adds provided journal entry to the player's journal and adds the quest to the active quests list. Similar to the mwscript function Journal.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "id", type = "tes3dialogue|string" },
			{ name = "index", type = "integer" },
			{ name = "speaker", type = "tes3mobileActor|tes3reference|string", optional = true, default = "tes3.mobilePlayer" },
			{ name = "showMessage", type = "boolean", optional = true, default = true, description = "If set, a message may be shown to the player." },
		},
	}},
	returns = {
		{ name = "wasUpdated", type = "boolean" },
	},
}