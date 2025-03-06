# Logging

The MWSE Logger library allows you to create a logger for your mod. It can be a helpful tool when debugging mods, and has designed to be simple to use. 


## Quickstart
<!-- The basic idea is to create a logger, and then use its methods to write messages to a log file. (The default file is `MWSE.log`.) -->

### Creating a Logger
For the vast majority of use-cases, it's enough to write:
```lua
-- Import the library.
local Logger = require("logger")
-- Create a new logger.
local log = Logger.new()
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
[My Awesome Mod | main.lua:6  | DEBUG] my message
```
The information in the header says that this log message was written on line 6 of the `main.lua` file of a mod called `"My Awesome Mod"`.
All of this information is captured automatically behind the scenes. In particular, the mod name will be loaded from your mod's [metadata file](../guides/metadata.md) if it has one.

### Writing More Useful Log Messages
In general, you'll likely want to log the values of various local variables you have floating around in your code. The logging framework makes this easy. For example:
```lua
-- Any kind of table works.
local myData = {x = 1, y = 20, date = "2025-03-02"}
log:debug("myData = %s.", myData)
```
outputs
```
[My Awesome Mod | main.lua:10  | DEBUG] myData = { date = "2025-03-02", x = 1, y = 20 }.
```
Internally, the logging methods convert their arguments into useful, human-readable text representations and then pass them to [`string.format`](/apis/string/#stringformat).

This wraps up the quickstart guide. But there are still a few useful things you may want to know about before publishing your mod.



## In more Detail

### Log Levels

There are 5 logging levels:

1. `ERROR`: Something bad happened.
2. `WARN`: Something bad _almost_ happened, or something bad _did_ happen but it's not a big deal.
3. `INFO`: Something normal and expected has happened. (i.e., the mod was loaded.) 
4. `DEBUG`: Used to record the inner workings of a mod. Useful for troubleshooting. 
5. `TRACE`: Basically like `DEBUG`, but more extreme. Also useful for debugging code that gets run very frequently.

The `Logger:error` method creates an `ERROR` message. Similarly, `Logger:debug` creates a `DEBUG` message, and so on.
There is also a shorthand syntax for `DEBUG` messages: you can simply write `log("my message")`.

Only logs at or below the current log level will be printed to the log file. For example, if the log level is set to `INFO`, then `INFO`, `WARN` and `ERROR` messages will be logged, but `TRACE` and `DEBUG` messages will not.
This has two advantages:

1. Logging messages have basically no performance impact when the user has disabled them.
2. The `MWSE.log` file is (ideally) not flooded by tons of logging messages that the user does not care about.



### Loggers Synchronize their Settings
Each file of your mod gets its own unique logger, corresponding to that filepath. 
This helps to make it easy to track down the exact origin of a logging statement. 
But this also introduces a potential problem: how do you make sure all the loggers synchronize their settings properly?

The answer is that you don't have to! The logging framework ensures that all loggers synchronize their data automatically.
Whenever you update the logging level of one logger, all the other loggers have their logging levels updated as well.
The same goes for changing the `modName`, whether or not to include timestamps, and various other formatting parameters.

### Passing Functions to the Logging Methods

The following situation is fairly common: you have a variable that you would like to include in a log statement, but it wouldn't be very useful to log it in its current form. You would ideally like to call a function in order to process that variable and get out a more useful representation of it, but this computation would be wasteful if the logging level isn't high enough to print the desired message. 

To solve this problem, the logging framework allows you to lazily (i.e., only when necessary) evaluate functions by passing them as arguments. For example:
```lua
local skillId = tes3.skill.heavyArmor
log("The skill name is %s", tes3.getSkillName, skillId)
-- is basically the same as
log("The skill name is %s", tes3.getSkillName(skillId))
```
The key difference is that, in the first case, `tes3.getSkillname(skillId)` is computed _after_ checking that the logging level is sufficient to print out the log statement.

You can also define your own functions in-place:
```lua
log("Fargoth's sneak level is %s", function()
	-- Very expensive function! It would be a shame to waste its output.
	local fargoth = tes3.getReference("fargoth")
	if fargoth then	
		return fargoth.object.skills[tes3.skill.sneak + 1]
	else
		return "unknown!"
	end
end)
```
yields
```
[My Awesome Mod | main.lua:29  | DEBUG] Fargoth's sneak level is 32
```

You can mix and match functions and other arguments as desired.
For example if `func` is a function that takes two arguments, then
```lua
local skillLevel = tes3.mobilePlayer[skillId + 1].current
log("%s %s %s", a, func, b, c, d)
-- becomes
log("%s %s %s", a, func(b, c), d)
```
But be warned, some functions might be defined to take more arguments than you might initially expect. 
For example, `json.encode` takes two arguments, despite typically only being called with a single argument.
Consider:
```lua
-- Bad: json.encode gets called with "hello!" as the second argument.
log("table = %s. message = %s", json.encode, someTable, "hello!")
-- OK: Instead calls json.encode(sometable, nil)
log("table = %s. message = %s", json.encode, someTable, nil, "hello!")
```
Use this functionality with care.

## Customizing Your Logger

You can customize the appearance of your logging messages in a number of ways.

### Include a Timestamp
If you set `log.includeTimestamp = true`, then all log messages will include a timestamp. 
This is taken relative to the time the game launched. For example:
```lua
log.includeTimestamp = true
log("In main.lua!")
event.register("initialized", function (e)
	log("Game has initialized!")
end)
```
yields
```
[My Awesome Mod | main.lua:40  | DEBUG | 00:00.343] In main.lua!
... -- other messages
[My Awesome Mod | main.lua:42  | DEBUG | 00:04.250] Game has initialized!
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
You can then compare the timestamps in the corresponding log messages to get an idea of how long it takes your function to execute.
Log statements can also be sprinkled into various parts of your function to get a more granular view of how long it takes to execute each part of the function.

### Set a custom output file.
You can ask your logger to record its output in a separate file. To do so, write
```lua
log.SetOutputFile("my file")
```
This will ensure your log messages get written to `Data Files/MWSE/logs/my file.log`. 
You can also write
```lua
log.SetOutputFile(true)
```
to automatically generate an output file based on the name of your mod. 
For example, if your mod was named "My Awesome Mod", then the above code would result in your logging statements
being written to `Data Files/MWSE/logs/My Awesome Mod.log`.

## TODO: write the rest of the guide

### Log Colors

In the MCM page of the script extender, there is an option to enable log colors. 
This will display logs in different colors according to their log level.


## Registering and using your Logger

```lua
local logger = require("logging.logger")
local log = logger.new{
	name = "Test Mod",
	logLevel = "TRACE",
	logToConsole = true,
	includeTimestamp = true,
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
local Logger = require("Logger")
local log = Logger.getLogger("Test Mod")

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
		log.level = self.variable.value
	end
}
```
