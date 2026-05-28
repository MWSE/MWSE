return {
	type = "function",
	description = [[Saves a serializable table to Data Files\\MWSE\\{`fileName`}.json, using `json.encode`.]],
	arguments = {
		{ name = "fileName", type = "string" },
		{ name = "object", type = "table" },
		{ name = "config", type = "jsonConfig", optional = true },
	},
}
