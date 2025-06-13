return {
	type = "function",
	description = [[Inserts a given value through BinaryInsert into an array-style `table` `t`, assuming `t` was sorted by `comp`.

If 'comp' is given, then it must be a function that receives two values in `t` and returns `true` when the first is less than the second.
i.e., `comp(a,b) == true` would mean `a` is less than `b`.

If you want to sort in reverse order, you may set `comp = function(a, b) return a > b end`.

This function will return the index where `value` was inserted into `t`.]],
	generics = {
		{ name = "valueType" }
	},
	arguments = {
		{ name = "t", type = "valueType[]" },
		{ name = "value", type = "valueType" },
		{ name = "comp", type = "fun(valueType, valueType):boolean", optional = true },
	},
	valuetype = "number",
}