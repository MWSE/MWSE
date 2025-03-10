return {
	type = "function",
	description = [[Creates a new logger based on the input parameters.]],
	arguments = {{
		name = "params",
		type = "table",
		optional = true,
		tableParams = {
			{ name = "modName", 	type = "string", optional = true, description = "The name of MWSE mod associated to this Logger. This will be retrieved automatically if not provided." },
			{ name = "moduleName", 	type = "string", optional = true, description = "The module this Logger is associated with. This can be useful for distinguishes which parts of your mod produce certain log messages. This will be displayed next to the name of the mod, in parentheses."},
			
			{ name = "level", 	     type = "mwseLogger.LOG_LEVEL", optional = true, default = "mwseLogger.LOG_LEVEL.DEBUG", description = "The logging level for all loggers associated to this mod." },
			{ name = "logToConsole", type = "bool", 			    optional = true, default = false, 					     description = "Should the output also be written to the in-game console?" },
			{ name = "outputFile", 	 type = "bool|string", 			optional = true, default = false, 					     description = "The path of the output file to write log messages in. This path is taken relative to `Data Files/MWSE/logs/`. If not provided, log messages will be written to `MWSE.log`. If `true`, then the `modDir` will be used as the output path." },
			
			{ name = "includeLineNumber", type = "bool", optional = true, default = true, description = "Should line numbers be included in logging messages?" },
			{ name = "includeTimestamp", type = "bool", optional = true, default = true, description = "Should timestamps be included in logging messages? The timestamps are relative to the time that the game was launched." },
			{ name = "abbreviateHeader", type = "bool", optional = true, default = false, description = "Should the headers be abbreviated?" },
			{ name = "formatter", type = "fun(self: Logger, record: mwseLogger.Record, ...: string|any|fun(...): ...): string", optional = true, description = "A custom formatter. This lets you customize how your logging messages are formatted. If not provided, the default formatter will be used." },
			
			{ name = "modDir", 		type = "string", optional = true, description = "The directory this mod resides in. This will be retrieved automatically if not provided. This parameter should not be used in the vast majority of situations."},
			{ name = "filepath", 	type = "string", optional = true, description = "The path of the file that this Logger is used in. This will be retrieved automatically if not provided. This path is relative to `modDir`. This parameter should not be used in the vast majority of situations." },
		},
	}},
	returns = {
		{ name = "log", type = "mwseLogger", description = "The newly created logger." },
	},
}
