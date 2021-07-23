return {
	type = "function",
	description = [[Sets an object (of any kind) to be sourceless, which are objects the game does not store in savegames. This can be useful for mod-created temporary objects which are not necessary to save.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "object", type = "tes3baseObject", description = "" },
			{ name = "sourceless", type = "boolean", optional = true, default = true, description = "" },
		},
	}},
}
