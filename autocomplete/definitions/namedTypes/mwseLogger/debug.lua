return {
	type = "method",
	description = [[
Logs a debug message, if the logging level is `DEBUG` or higher.
The simplest way to use this method is to log ordinary strings to the logging file:
```lua
local Logger = require("Logger")
local log = Logger.new{ level = Logger.LOG_LEVEL.DEBUG }
log:("This is a string!")
```
results in the following message being printed to `MWSE.log`:
```
[My Awesome Mod | main.lua:3  | DEBUG] This is a string!
```
Notice that the mod name, filepath, and line number were retrieved automatically during logger construction. 

When supplying more than one argument to this function, the arguments will be prettyprinted and then passed to `string.format`. 
See the examples for more information.
	]],
	arguments = {
		{ name = "...", type = "any", optional = true,
			description = [[
Logging arguments. These are handled as follows:

	- Things with a `tostring` metamethod are printed according to that metamethod.
		- This includes all primitive types, such as `string`s and `number`s.
	- `table`s without a `tostring` metamethod are prettyprinted using the `inspect` library.
	- `function`s are evaluated lazily, consuming subsequent inputs based on their definition.
		- (For example, if a function takes 2 arguments, then it will consume the next two arguments.)

Additionally, there are more than two formatting arguments (after lazily evaluating all functions),
then `string.format` will be called on those arguments.
]]
		},
	},
	examples = {
		["lazyEvaluateFunctions"] = {
			title = "Lazily evaluate logging parameters",
			description = '\z
				The documentation of `tes3.getReference` says that it is a slow operation and should be used sparingly. \z
				Suppose that, for whatever reason, we only need to call this function in order to print helpful log messages. \z
				If the end-user does not have their logging level set to `DEBUG` or higher, then the following code will result in a \z
				lot of wasted computation: \z
				```lua \z
				local myData = {x = 1, y = 20, date = "2025-03-02"} \z
				log:debug("fargoth = %s. myData = %s", tes3.getReference("fargoth"), myData) \z
				```\z
				To get around this, we can lazily evaluate functions passed as log parameters.\z
			'
		},
		["stringFormatArguments"] = {
			title = "Logging parameters get prettyprinted and formatted with `string.format`",
		},
	},
}
