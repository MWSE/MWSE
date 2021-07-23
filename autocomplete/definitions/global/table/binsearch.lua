return {
	type = "function",
	description = [[Performs a binary search for a given value.

If the value is found: It returns a table holding all the mathing indices (e.g. { startindice,endindice } ) endindice may be the same as startindice if only one matching indice was found

If compval is given: then it must be a function that takes one value and returns a second value2, to be compared with the input value, e.g.: compvalue = function( value ) return value[1] end

If reversed is set to true: then the search assumes that the table is sorted in reverse order (largest value at position 1) note when reversed is given compval must be given as well, it can be nil/_ in this case

Return value: on success: a table holding matching indices (e.g. { startindice,endindice } ) on failure: nil]],
	arguments = {
		{ name = "t", type = "table" },
		{ name = "value", type = "unknown" },
		{ name = "compval", type = "unknown", optional = true },
		{ name = "reversed", type = "unknown", optional = true },
	},
	valuetype = "table",
}