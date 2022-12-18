# Logging

The MWSE Logger library allows you to register a logger for your mod. It provides the following features:

- Mod-specific log levels
- Change log level via MCM
- Log messages can be color coded according to log level (via MWSE MCM)
- Optional setting to print messages to a separate log file
- Optional setting to print messages to the in-game console

## Log Levels

Log levels are ordered as follows: `TRACE`, `DEBUG`, `INFO`, `WARN`, and `ERROR`. Only logs at or below the current log level will be printed to the log file. For example, if log level is set to `INFO`, then `INFO`, `WARN` and `ERROR` messages will be logged, but `TRACE` and `DEBUG` messages will not.

## Log Colors

In the MWSE MCM, there is an option to enable log colors. This will display logs in different colors according to their log level when tailing the log in the console. However, it is recommended that non-modders keep this turned off, as it makes the log file harder to read in a regular text editor.

### Log Level Colors:

- `TRACE`: Bright White
- `DEBUG`: Green
- `INFO`: White
- `WARN`: Yellow
- `ERROR`: Red


## Registering and using your Logger

```lua
local logger = require("logging.logger")
local log = logger.new{
	name = "Test Mod",
	logLevel = "TRACE",
	logToConsole = true,
}
log:trace("This is a trace message")
log:debug("This is a debug message")
log:info("This is an info message")
log:warn("This is a warn message")
log:error("This is an error message")

log:setLogLevel("INFO")

-- To disable logging to the in-game console, set the logToConsole field to false
log.logToConsole = false

-- After this point no logging messages will be logged to the in-game console
```

## Using your logger in different source files

In your main.lua, place the logger creation before other source files are included or required. This is to ensure the logger is created and accessible to these other source files.

In the other source files:
```lua
local logger = require("logging.logger")
local log = logger.getLogger("Test Mod")

log:info("This is an info message")
```

## Creating an MCM to control Log Level

In your MCM config, create a dropdown with the following options:
```lua
settings:createDropdown{
	label = "Logging Level",
	description = "Set the log level.",
	options = {
		{ label = "TRACE", value = "TRACE"},
		{ label = "DEBUG", value = "DEBUG"},
		{ label = "INFO", value = "INFO"},
		{ label = "WARN", value = "WARN"},
		{ label = "ERROR", value = "ERROR"},
		{ label = "NONE", value = "NONE"},
	},
	variable = mwse.mcm.createTableVariable{ id = "logLevel", table = mcmConfig },
	callback = function(self)
		log:setLogLevel(self.variable.value)
	end
}
```
