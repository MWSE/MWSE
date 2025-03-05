--- A logging record. Contains information about the log level and the line number.
---@class Logger.Record
---@field stackLevel integer how far up the stack we're being called
---@field level Logger.LEVEL logging level
---@field lineNumber integer|false the line number, if enabled for this logger
---@field timestamp number|false the timestamp of this message
return {
	type = "value",
	description = "The timestamp that was created when this logging messages was created. This is obtained directly from `socket.gettime()`. \z
		In particular, it captures the current real-world time, rather than the amount of time since the game launched.that triggered this record to be created.\n\z
		\n\z
		Will be `false` if the `Logger.includeTimetamp` is `false`.",
	valuetype = "number|false",
}
