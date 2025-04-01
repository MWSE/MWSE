return {
	type = "method",
	description = [[Set the log level. 
You can pass in either a string representation of a logging level, or the corresponding numerical constant found in the `mwse.LOG_LEVEL` table.
The options are: `"TRACE"`, `"DEBUG"`, `"INFO"`, `"WARN"`, `"ERROR"` and `"NONE"`.

This function does exactly the same thing as writing `log.level = newLogLevel`. 
Use whichever one you prefer.
]],
	arguments = {
		{ name = "newLogLevel", type = "mwseLogger.LOG_LEVEL" }
	}
}
