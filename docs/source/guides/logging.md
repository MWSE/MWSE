# Logging

The MWSE Logger library allows you to create a logger for your mod. It can be a helpful tool when debugging mods, and is designed to be simple to use.


## Quickstart

### Creating a Logger
For the vast majority of use-cases, it's enough to write:
```lua
local log = mwse.Logger.new()
```

### Writing Log Messages

To start debugging, set the logging level to be `DEBUG` or higher:
```lua
log.level = "DEBUG"
```
Typically, the above would be done by changing an MCM setting. And that is definitely how you should do it in practice, but this is the quickstart guide!

You can now write logging messages by typing:
```lua
log:debug("my message")
```
The above message will then be written to `MWSE.log` in the following format:
```
[My Awesome Mod | main.lua | DEBUG] my message
```
The information in the header says that this log message was written in the `main.lua` file of a mod called `"My Awesome Mod"`. All of this information is captured automatically behind the scenes. In particular, the mod name will be loaded from your mod's [metadata file](../guides/metadata.md) if it has one.

### Writing More Useful Log Messages
In general, you'll likely want to log the values of various local variables you have floating around in your code. The logging framework makes this easy. For example:
```lua
-- Any kind of table works.
local myData = {x = 1, y = 20, date = "2025-03-02"}
log:debug("myData = %s.", myData)
```
outputs
```
[My Awesome Mod | main.lua | DEBUG] myData = { date = "2025-03-02", x = 1, y = 20 }.
```
Internally, the logging methods convert their arguments into useful, human-readable text representations and then pass them to [`string.format`](../apis/string.md#stringformat).

This wraps up the quickstart guide. But there are still a few useful things you may want to know about before publishing your mod.


### Multifile Setups
It's common for larger mods to split code into multiple files. The Logger API is designed with this is mind and makes it easy to track down which logging message originated from which file. No extra work is required to support multiple files. Just write
```lua
local log = mwse.Logger.new()
```
in each file you want a `Logger`. The rest will be taken care of behind the scenes. For example, consider the following code:
```lua
local log = mwse.Logger.new()
log:info("Hello world")
```
If this code is run in `mods/My Awesome Mod/main.lua`, then the message would be:
```
[My Awesome Mod | main.lua | INFO] Hello world
```
Meanwhile, if the same code was run in `mods/My Awesome Mod/someOtherFile.lua`, the message would be
```
[My Awesome Mod | someotherfile.lua | INFO] Hello world
```
To make things simpler, all loggers created for a mod will synchronize their settings. For example, if you update `log.level` in `mods/My Awesome Mod/main.lua`, then the logging level will also be updated in `mods/My Awesome Mod/someOtherFile.lua`.

### Creating an MCM to Control Log Level

To configure your logging level use the `LogLevelOptions` MCM setting. It allows the user to change the logging level inside a dropdown:

```lua
settings:createLogLevelOptions{
	config = mcmConfig,
	configKey = "logLevel",
}
```

## In more Detail

### Log Levels

There are 5 logging levels:

1. `ERROR`: Something bad happened.
2. `WARN`: Something potentially bad happened, or something bad _almost_ happened.
3. `INFO`: Something normal and expected has happened. (i.e., the mod was loaded.)
4. `DEBUG`: Used to record the inner workings of a mod. Useful for troubleshooting.
5. `TRACE`: Basically like `DEBUG`, but more extreme. Also useful for debugging code that gets run very frequently.

Log messages may be written as follows:
```lua
log:trace("This is a TRACE message")
log:debug("This is a DEBUG message")
log("This is also a DEBUG message") -- Shorthand syntax.
log:info("This is an INFO message")
log:warn("This is a WARN message")
log:error("This is an ERROR message")
```

Only logs at or below the current log level will be printed to the log file. For example, if the log level is set to `INFO`, then `INFO`, `WARN` and `ERROR` messages will be logged, but `TRACE` and `DEBUG` messages will not. This has two advantages:

1. Logging messages have basically no performance impact when the user has disabled them.
2. The `MWSE.log` file is (ideally) not flooded by tons of logging messages that the user does not care about.



### Loggers Synchronize their Settings

Each file of your mod gets its own unique logger, corresponding to that filepath. This helps to make it easy to track down the exact origin of a logging statement.  But this also introduces a potential problem: how do you make sure all the loggers synchronize their settings properly?

The answer is that you don't have to! The logging framework ensures that all loggers synchronize their data automatically. Whenever you update the logging level of one logger, all the other loggers have their logging levels updated as well. The same goes for changing the `modName`, whether or not to include timestamps, and various other formatting parameters.

### Passing Functions to the Logging Methods

The following situation is fairly common: you have some data you want to include in a log statement, but you'd like to transform it in some way before logging it. Examples include things like logging the keys of a table, logging the skill names of a table of `tes3.skill` IDs, etc. In other words, you would like to transform some data before including it in a log message, but it would be rather inefficient to transform the data if the log message isn't going to be printed anyway.


To solve this problem, the logging framework allows you to lazily (i.e., only when necessary) compute the values passed to logging messages. For example: evaluate functions by passing them as arguments. For example:
```lua
local skillId = tes3.skill.heavyArmor
local myTable = { a = 1, b = 2, c = 3, d = 4}
log("The keys of my table are: %s.", function()
	return json.encode(table.keys(myTable))
end)
-- is basically the same as
log("The keys of my table are: %s.", json.encode(table.keys(myTable)))
```
The key difference is that, in the first case, `json.encode(table.keys(myTable))` is computed _after_ ensuring that the logging level is high enough.

In general,
```lua
log(formatString, func, ...)
```
becomes
```lua
if log.level >= mwse.logLevel.debug then
    log(formatString, func(...))
end
```
This means that it's also possible to define `func` somewhere else and then re-use it in multiple log statements. For example, you can take advantage of the fact that tables are pretty-printed to write the log statement from before as:
```lua
log("The keys of my table are: %s.", table.keys, myTable)
```
It's also possible for `func` to return multiple formatting parameters.

One other form of lazy evaluation is supported as well:
```lua
log(func, ...)
-- expands to
log(func(...))
```
This means it's possible to have a function return the format string as well.

## Customizing Your Logger

You can customize the appearance of your logging messages in a number of ways.

### Include a Timestamp
If you set `log.includeTimestamp = true`, then all log messages will include a timestamp. This is taken relative to the time the game launched. For example:
```lua
log.includeTimestamp = true
log("In main.lua!")
event.register("initialized", function (e)
	log("Game has initialized!")
end)
```
yields
```
[My Awesome Mod | main.lua | DEBUG | 00:00.343] In main.lua!
... -- other messages
[My Awesome Mod | main.lua | DEBUG | 00:04.250] Game has initialized!
```
This can also be used as an easy way to time certain functions:
```lua
local functionThatMightBeTooSlow()
	log:trace("started functionThatMightBeTooSlow")
	... -- actually do stuff here
	log:trace("ending functionThatMightBeTooSlow")
	return whatever
end
```
You can then compare the timestamps in the corresponding log messages to get an idea of how long it takes your function to execute. Log statements can also be sprinkled into various parts of your function to get a more granular view of how long it takes to execute each part of the function.

### Set a custom output file.
You can ask your logger to record its output in a separate file. To do so, write:
```lua
log:setOutputFile("my file")
```
This will ensure your log messages get written to `Data Files/MWSE/logs/my file.log`. You can also write:
```lua
log:setOutputFile(true)
```
to automatically generate an output file based on the name of your mod. For example, if your mod was named "My Awesome Mod", then the above code would result in your logging statements being written to `Data Files/MWSE/logs/My Awesome Mod.log`.


### Log Colors

In the MCM page of the script extender, there is an option to enable log colors.  This will display logs in different colors according to their log level.

### Line Numbers

In the MCM page of the script extender, there is an option to include line numbers in log messages. This can make it easier to track down a specific location in which a log message was written. For example, writing the following log statement on line 215
```lua
log("Here!")
```
prints the following text to `MWSE.log`:
```
[My Awesome Mod | main.lua:215 | DEBUG] Here!
```
Note that enabling line numbers does incur a performance penalty, which is why it is not enabled by default.

## Creating a new Logger
The simplest way to create a new logger is to simply call `mwse.Logger.new()` and have all the relevant information be retrieved automatically. But it's also possible to pass additional parameters to the `new` function, all of which are optional:
```lua
local log = mwse.Logger.new{
	-- Manually specify the name of your mod.
	-- This can be done if aren't happy with the way that the logger automatically retrieves the name of your mod.
	modName = "My Mod",
	-- A way to differentiate loggers that are part of the same file.
	-- Can be any string.
	moduleName = "Skills Component",
	-- Set a log level on creation. But it's typically best to manage log levels entirely through the MCM.
	level = "ERROR",
	-- Print the log messages in a separate file.
	-- Can be either true, false, or a string specifying the name of the file.
	outputFile = true,
	-- Include a timestamp in log messages.
	includeTimestamp = true,
	-- Shorten the header portion of the logging messages.
	abbreviateHeader = true,
	-- Advanced option: specify a custom formatting function for your log messages.
	-- This can be used if you would like to change how log messages are printed.
	formatter = myFormattingFunction
}
```
All of the above settings only need to be specified (at most) once, because loggers synchronize their settings. This means that, for example, it's enough to specify the `modName` parameter in a single file.

In your main.lua, place the logger creation before other source files are included or required. This is to ensure the logger is created and accessible to these other source files.

## Advanced: Customizing the formatter

Under the hood, logging messages are printed by executing code that (in its simplest form) is analogous to:
```lua
print(self.formatter(self, logRecord, ...))
```
The `formatter` is responsible for piecing together all of its arguments to generate a coherent (and helpful) logging message. It receives the following parameters:

- `self`: The `mwseLogger` that issued the logging statement.
- `logRecord`: A [`mwseLogger.Record`](../types/mwseLogger.Record.md) created by the logging statement. This is basically just a table that, among other things, stores:
	- The line number that the log message originated from. This is only available if the relevant MWSE setting is enabled.
	- The `mwseLogger.logLevel` of the log statement. (e.g., this will be `logLevel.debug` if the log statement was created by the `Logger:debug` method.)
	- A timestamp that marks when the logging call was issued. This is only available if the logger in question had `log.includeTimestamp == true`.
- `...`: This refers to the actual parameters passed to the logging functions (e.g., to `Logger:debug(...)`).

Several of the features mentioned in this guide (e.g. lazy function evaluation, prettyprinting tables) are due to the behavior of the default formatter. As such, they can be customized by changing the `formatter` field of your `Logger`.
!!! warning
	The `formatter` field is also responsible for printing the "header" that appears in braces before the body of a log statement. In other words, it is the responsibility of the `formatter` to print the following part of logging messages:
	```
	[My Awesome Mod | main.lua | DEBUG | 00:04.250]
	```
	When writing a custom formatter, it is your responsibility to ensure a header is preprended to the output of your custom formatter. If you would prefer to avoid rewriting the "header" yourself, you can use the protected `Logger:makeHeader` method to use the default header. This means your formatter would look something like:
	```lua
	---@param self mwseLogger
	---@param record mwseLogger.Record
	---@param ... any
	local function myFormatter(self, record, ...)
		-- Use the default header.
		local header = self:makeHeader(record)

		---@type string
		local message

		do -- Use `self`, `record`, and `...` to generate the logging `message`.
			...
		end

		-- Prefix the message with the default header.
		return header .. message
	end
	```

The definition of the default formatter can be found at `MWSE/core/lib/Logger/formatters.lua`. The `formatters.lua` file also includes another formatter, labeled `expandAllFunctions` that illustrates how custom formatters can be used to alter the behavior of the logging methods. In particular, the `expandAllFunctions` formatter will lazily-evaluate _all_ functions that are passed to the logging methods.


!!! example "Example: Creating a minimal formatter"
	The following example illustrates how a custom `formatter` can be used to make the logging methods behave more similarly to `string.format`:
	```lua
	local log = mwse.Logger.new{
		---@param self mwseLogger
		---@param record mwseLogger.Record
		---@param msg string|any
		---@param ... any
		formatter = function(self, record, msg, ...)
			-- Use the default header.
			local header = self:makeHeader(record)

			-- Make sure the first parameter is a string.
			local outputMessage = tostring(msg)

			-- If there were multiple arguments passed, call `string.format`.
			if select("#", ...) > 0 then
				outputMessage = string.format(msg, ...)
			end

			-- Prefix the message with the default header.
			return header .. outputMessage
		end
	}
	```
	Notice how in the above code, we used `msg` to capture the first variadic argument. The above formatter would result in these log messages
	```lua
	log:info("My table: %s", {a = 1})
	log:info("Encoded table: %s", json.encode, {a = 1})
	```
	being printed as:
	```
	[my awesome mod | main.lua | INFO] My table: table: 0x18b33be8
	[my awesome mod | main.lua | INFO] Encoded table: function: 0x18515738
	```
	In particular, the above formatter does not prettyprint tables nor does it lazily evaluate its function arguments.

Note that formatters are synchronized between all loggers belonging to the same mod, so you only need to update the formatter in one place. If customizing the formatter, it is best to do it at the very beginning of your `main.lua` file, before importing any other files. This ensures that all logging messages will use your formatter.