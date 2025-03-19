return {
	type = "value",
	description = [[The plain text of the script. Note that line endings do not match the default lua line endings. This requires file IO, and is slow. If `recompile` is used to change the script at runtime, this will not be accurate.]],
	readOnly = true,
	valuetype = "string",
}