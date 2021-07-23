return {
	type = "function",
	description = [[Sets the writing mode for a file.]],
	arguments = {
		{ name = "file", type = "ioFile", description = "The file to set mode for." },
		{ name = "mode", type = "string", description = "The file's new mode. Can be either \"binary\" or \"text\"." },
	},
	returns = {
		{ name = "success", type = "boolean|nil", description = "true in case of success, or nil on an error." },
		{ name = "errorOrOldMode", type = "string", description = "If succesful, this will be the previous file mode used. In the case of an error, a string describing the issue." },
	},
}