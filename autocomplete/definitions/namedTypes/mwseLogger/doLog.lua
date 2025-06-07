return {
	type = "method",
	deprecated = true,
	description = [[
		Returns true if the messages of the given log level will be logged.
		This method is deprecated. It is recommended to either use 
		[lazy evaluation](../guides/logging.md#passing-functions-to-the-logging-methods)
		or check the logging level directly, e.g.:
		```lua
		if log.level >= mwse.logLevel.debug then 
			-- do stuff
		end
		```
	]],
	arguments = {
		{ name = "logLevel", type = "mwseLoggerLogLevel", description = [[Options are: "TRACE", "DEBUG", "INFO", "WARN", "ERROR" and "NONE".]] },
	},
	returns = {
		{ name = "doLog", type = "boolean" }
	}
}
