return {
	type = "class",
	-- note: code lines are indented according which to which actual line they belong to.
	description = "\z
		A class to facilitate printing log messages. \z
			A new one can be constructed by simply calling `require(\"logger\").new()`. \z
			Log messsages include: \n\n\z
		1) Mod name. This is retrieved automatically if not explicitly provided during logger construction. \n\z
		2) The path of the file for which the logging message originated. This is retrieved automatically.\n\z
		3) The line number on which the logging message appeared. This is retrieved automatically.\n\z
		4) Optionally: a timestamp indicating the time since game launch.\n\z
		5) Optionally: a \"module name\" for the module that issued the logging message.\n\z
		If desired, this must be provided during logger construction. See [`moduleName`](#modulename)\n\z
		\n\z
		\n\z
		Logging messages will be passed through `string.format` if desired, \z
		and any `table` and `userdata` arguments will be prettyprinted. \z
		Additionally, it is possible to lazily evaluate functions that are supplied to the logging methods. \z
		For more information on how this works, see the documentation for [`Logger:debug`](#debug).\z
		\n\z
		\n\z
		Several logging setting are synchronized between the different loggers used by the same mod, so they only need to be updated in one place.\z
	",
}
