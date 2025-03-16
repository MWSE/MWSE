return {
	type = "method",
	description = [[Finds a child element matching the `id` argument. Searches children recursively. Returns the first child element with a matching id, or `nil` if no match found.]],
	arguments = {
		{ name = "id", type = "string|number", description = "The id to search for." },
	},
	valuetype = "tes3uiElement|nil",
}