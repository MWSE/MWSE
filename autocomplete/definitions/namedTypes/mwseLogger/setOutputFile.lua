return {
	type = "method",
	description = [[Set the output file. If set, logs will be sent to a file of this name.

This function does exactly the same thing as `log.outputFile = newOutputFile`. 
Use whichever one you prefer.
]],
	arguments = {
		{ name = "outputFile", type = "string|boolean", description = "If `true`, then the `modName` field will be used as the filepath. If `false`, no custom output file will be used." }
	}
}
