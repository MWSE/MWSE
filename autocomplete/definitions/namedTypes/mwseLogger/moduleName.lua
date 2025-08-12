return {
	type = "value",
	description = [[
Associates a logger with a particular "module". 
The `moduleName` will be printed in logging messages next to the `modName`.
What does and does not constitute a "module" is entirely subjective. Use this field as you please. 
This can be useful if the `filepath` alone is not enough to distinguish what code is reponsible for issuing a log message.
For example, the MWSE dependency management system uses a `moduleName` to alert the user about which mod had a dependency problem.
	]],
}
