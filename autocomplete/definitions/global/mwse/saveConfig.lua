return {
	type = "function",
	description = [[Saves a config table to Data Files\\MWSE\\config\\{fileName}.json. The config is converted to JSON during saving.]],
	arguments = {
		{ name = "fileName", type = "string", description = "Usually named after your mod." },
		{ name = "config", type = "table", description = "The config table to save." },
		{ name = "jsonOptions", type = "jsonConfig", optional = true, description = "Encoding options. These get passed to the `dkjson` encoder." },
	},
}
