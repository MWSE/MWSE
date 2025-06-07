return {
	type = "method",
	description = [[Changes the `moduleName` field of this logger.
	
This function does exactly the same thing as writing `log.moduleName = newModuleName`. 
Use whichever one you prefer.
]],
	arguments = {
		{ name = "newModName", type = "string" }
	}
}
