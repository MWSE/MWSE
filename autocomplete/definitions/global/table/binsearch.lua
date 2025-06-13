return {
	type = "function",
	description = [[Performs a binary search for a given `value` inside a specified array-style `table` `tbl`.

If the `value` is in `tbl`, then its corresponding `index` will be returned. Otherwise, this function will return `nil`.
If `findAll == true`, then this `binsearch` will return the lowest and highest indices that store `value`. (These indices will be equal if there is only one copy of `value` in `tbl`.)

You can optionally provide a `comp` function. If provided, `binsearch` will treat `tbl` as if it had been sorted by `table.sort(tbl, comp)`.
]],
	generics = {
		{ name = "valueType" }
	},
	arguments = {
		{ name = "tbl", type = "valueType[]" },
		{ name = "value", type = "valueType", description = "The value to search for." },
		{ name = "comp", type = "fun(valueType, valueType):boolean", optional = true, description = "The function used to sort `tbl`. If not provided, then the standard `<` operator will be used." },
		{ name = "findAll", type = "boolean", optional = true, default = false, description = "If true," },
	},
	returns = {
		{ name = "index", type = "integer|nil", description = "An `index` such that `tbl[index] == value`, if such an index exists. `nil` otherwise. If `findAll == true`, this will be the smallest index such that `tbl[index] == value`." },
		{ name = "highestMatch", type = "integer|nil", description = "If a match was found, and if `findAll == true`, then this will be the largest `index` such that `tbl[index] == vale`. `nil` otherwise." },
	},
}