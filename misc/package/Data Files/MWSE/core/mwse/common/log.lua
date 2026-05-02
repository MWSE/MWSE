local logger = require("logging.logger")
return logger.new({
	name = "MWSE",
	logLevel = "TRACE",
	logToConsole = false,
	includeTimestamp = true,
})