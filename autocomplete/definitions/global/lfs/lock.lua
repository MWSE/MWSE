return {
	type = "function",
	description = [[Locks a file or a part of it. This function works on open files; the file handle should be specified as the first argument.]],
	arguments = {
		{ name = "filehandle", type = "ioFile", description = "The file handle to lock." },
		{ name = "mode", type = "string", description = "\"r\" for a read/shared lock, or \"w\" for a write/exclusive lock." },
		{ name = "start", type = "number", optional = true, description = "The starting point of the file to lock." },
		{ name = "length", type = "number", optional = true, description = "The length of the file, relative to start, to lock." },
	},
	returns = {
		{ name = "success", type = "boolean|nil", description = "true if the operation was successful, or nil in the case of an error." },
		{ name = "error", type = "string|nil", description = "In the case of an error, a string describing the issue. In particular, if the lock exists and is not stale it returns the \"File exists\" message." },
	},
}