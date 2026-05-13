return {
	type = "function",
	description = [[Returns true if the given MWSE object userdata still refers to a live engine object. This can be used to detect stale borrowed objects after the underlying object has been deleted. Returns false for anything that isn't a valid userdata.]],
	arguments = {
		{ name = "object", type = "any", description = "The object userdata to test." }
	},
	returns = {
		{ name = "valid", type = "boolean" },
	}
}
