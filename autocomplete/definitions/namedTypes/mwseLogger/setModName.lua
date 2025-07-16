return {
	type = "method",
	description = [[Changes the `modName` field of this logger.
	
This function does exactly the same thing as writing `log.modName = newModName`. 
Use whichever one you prefer.
]],
	arguments = {
		{ name = "newModName", type = "string" }
	}
}
