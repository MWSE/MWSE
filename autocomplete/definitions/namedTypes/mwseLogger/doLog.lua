return {
	type = "method",
	deprecated = true,
	description = [[
		Returns true if the messages of the given log level will be logged.
		The preferred way of doing this is to access `log.level` directly and compare it with the log level constants. E.g.,
		```lua
		local Logger = require("Logger")
		local log = Logger.new()
		if log.level >= mwse.LOG_LEVEL.DEBUG then 
			-- do sstuff
		end
		```
		However, in practice this shouldn't be all that necessary due to the ability to lazy-evaluate the arguments to the logging functions.
	]],
	arguments = {
		{ name = "logLevel", type = "mwseLoggerLogLevel", description = [[Options are: "TRACE", "DEBUG", "INFO", "WARN", "ERROR" and "NONE".]] },
	},
	returns = {
		{ name = "doLog", type = "boolean" }
	}
}
