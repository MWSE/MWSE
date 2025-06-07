return {
	type = "method",
	description = [[Gets a `string` representation of the current logging level.]],
	arguments = {
		{
			name = "level",
			type = "mwseLogger.logLevel",
			optional = true,
			description = "If provided, a string representation of this logging level will be returned. If `nil`, then a string representation of the current logging level will be returned.",
		},
	},
	returns = { { name = "levelString", type = "string" } },

}
