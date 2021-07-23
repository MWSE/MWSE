return {
	type = "function",
	description = [[Creates a lockfile (called lockfile.lfs) in path if it does not exist and returns the lock. If the lock already exists checks if it's stale, using the second parameter (default for the second parameter is INT_MAX, which in practice means the lock will never be stale. The lock object has a method to free it.]],
	arguments = {
		{ name = "path", type = "string", description = "The path to lock." },
	},
	returns = {
		{ name = "success", type = "lfsLock|nil", description = "The lock object to manage, or nil in the case of an error." },
		{ name = "error", type = "string|nil", description = "In the case of an error, a string describing the issue. In particular, if the lock exists and is not stale it returns the \"File exists\" message." },
	},
}