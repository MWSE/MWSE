return {
	type = "function",
	description = [[Creates a new array-style table that results from using `f` to filter out elements of an array-style table `t`. i.e., `table.filterarray(t,f)` will consist of only the pairs `i, v` of `t` for which `f(v)` was not `false` or `nil`.
Any additional arguments will be passed to `f`. For example, `table.filterarray(t, f, 10)` would call `f(v, 10)` on each value `v` of `t`.

When an element gets filtered out, the index of subsequent items will be shifted down, so that the resulting table plays nicely with the `#` operator and the `ipairs` function.]],
	arguments = {
		{ name = "t", type = "table" },
		{ name = "f", type = "fun(v: unknown, ...)", "The function to use when filtering values of `t`. (This is sometimes called a predicate function.)" },
		{ name = "...", type = "any", description = "Additional parameters to pass to `f`." },
	},
	returns = {
		{ name = "result", type = "table", description = "The result of using `f` to filter out elements of `t`." },
	}
}