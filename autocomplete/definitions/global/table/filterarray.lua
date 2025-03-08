return {
	type = "function",
	description = [[Creates a new array-style table that results from using `f` to filter out elements of an array-style table `arr`. i.e., `table.filterarray(arr, f)` 
will consist of only the pairs `i, v` of `arr` for which `f(i, v)` was not `false` or `nil`.
Any additional arguments will be passed to `f`. For example, `table.filterarray(arr, f, 10)` would call `f(i, v, 10)` on each value pair `i, v` of `arr`.

When an element gets filtered out, the index of subsequent items will be shifted down, so that the resulting table plays nicely with the `#` operator and the `ipairs` function.]],
	arguments = {
		{ name = "arr", type = "table" },
		{ name = "f", type = "fun(i: integer, v: unknown, ...): boolean", description = "The function to use when filtering values of `t`. (This is sometimes called a predicate function.)" },
		{ name = "...", type = "any", description = "Additional parameters to pass to `f`." },
	},
	returns = {
		{ name = "result", type = "table", description = "The result of using `f` to filter out elements of `t`." },
	}
}