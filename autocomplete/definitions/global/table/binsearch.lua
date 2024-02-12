return {
	type = "function",
	description = [[Performs a binary search for a given `value` inside a specified `table` `t`.

If the `value` is found in `t`, then a `table` providing the range of all matching indices is returned. (e.g. `{ startindice, endindice }`.) 
If only one matching index was found, then `startindice` will be the same as `endindice`.

If `value` is not found in `t`, then `nil` is returned.

If `compval` is given, then it must be a function that takes in an element of `t` and returns a value to use for comparisons.
For example, to compare arrays based on their first entry, you can write `compvalue = function(value) return value[1] end`.

Note that `compval` is different from the `comp` that is specified in the `bininsert` function.

If `reversed == true`, then the search assumes that `t` is sorted in reverse order (i.e., largest value at position 1).
Note that specifying `reversed` requires specifying `compval`. 
You can circumvent this by passing `nil` for `compval`. e.g., `binsearch(tbl, value, nil, true)`.]],
	arguments = {
		{ name = "t", type = "table" },
		{ name = "value", type = "unknown", description = "The value to search for." },
		{ name = "compval", type = "function", optional = true, description = "A function that returns the value to use in comparisons." },
		{ name = "reversed", type = "boolean", optional = true, description = "If true, then `binsearch` will assume `t` is sorted in reverse order." },
	},
	valuetype = "table",
}