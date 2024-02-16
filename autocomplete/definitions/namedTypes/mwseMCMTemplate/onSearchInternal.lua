return {
	type = "method",
	description = [[Performs searching routine for given `searchText`.]],
	arguments = {
		{ name = "searchText", type = "string" }
	},
	returns = {
		{ name = "result", type = "boolean", description = "True if given `searchText` matches this MCM template." }
	}
}
