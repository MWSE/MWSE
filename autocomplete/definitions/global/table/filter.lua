return {
	type = "function",
	description = [[Creates a new table that results from using `f` to filter out elements of `t`. i.e., `table.filter(t,f)` will consist of only the pairs `k, v` of `t` for which `f(k, v)` was not `false` or `nil`.
Any additional arguments will be passed to `f`. For example, `table.filter(t, f, 10)` would call `f(k, v, 10)` on each pair `k, v` of `t`.

!!! warning
 	Do not use this function on array-style tables, as it will not shift indices down after filtering out elements. Instead, you should use `table.filterarray` on array-style tables.
]],
	generics = {
		{ name = "keyType" },
		{ name = "valueType" },
	},
	arguments = {
		{ name = "t", type = "{ [keyType]: valueType }" },
		{ name = "f", type = "fun(k: keyType, v: valueType, ...):boolean", description = "The function to use when filtering values of `t`. (This is sometimes called a predicate function.)" },
		{ name = "...", type = "any", description = "Additional parameters to pass to `f`." },
	},
	returns = {
		{ name = "result", type = "{ [keyType]: valueType }", description = "The result of using `f` to filter out elements of `t`." },
	}
}