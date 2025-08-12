return {
	type = "value",
	description = [[Determines where logging messages are printed. If `false`, log messages are printed to `MWSE.log`. 
If it's a `string`, then logging messages will be printed to `Data Files/MWSE/logs/<log.outputFile>.log`.

Setting this to `true` is the same as writing `log.outputFile = log.modDir`.]],
	valuetype = "string|boolean",
}
