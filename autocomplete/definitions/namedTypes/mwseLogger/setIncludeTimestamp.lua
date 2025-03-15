return {
	type = "method",
	description = [[Changes the `includeTimestamp` field of this logger.
	
This function does exactly the same thing as `log.includeTimestamp = newIncludeTimestamp`. 
Use whichever one you prefer.
]],
	arguments = {
		{ name = "newIncludeTimestamp", type = "boolean" }
	}
}
