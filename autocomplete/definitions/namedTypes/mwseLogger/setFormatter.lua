return {
	type = "method",
	description = [[Changes the `formatter` field of this logger.

This function does exactly the same thing as writing `log.formatter = newFormatter`.
Use whichever one you prefer.
]],
	arguments = {
		{ name = "newFormatter", type = "fun(self: mwseLogger, record: mwseLoggerRecord, ...: string|any|fun(...): ...): string" }
	}
}
