return {
	type = "function",
	description = [[Sets the index of a given quest. Doesn't alter journal entries. Similar to the mwscript function SetJournalIndex.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "id", type = "tes3dialogue|string" },
			{ name = "index", type = "integer" },
			{ name = "showMessage", type = "boolean", optional = true, default = false, description = "If set, a message may be shown to the player." },
		},
	}},
	returns = {
		{ name = "wasSet", type = "boolean" },
	},
}