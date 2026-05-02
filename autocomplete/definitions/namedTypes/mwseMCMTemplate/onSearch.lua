return {
	type = "value",
	description = [[A custom search handler function. This function should return true if this mod Template should show up in search results for given `searchText` (it's in lowercase).]],
	valuetype = "nil|fun(searchText: string): boolean",
}
