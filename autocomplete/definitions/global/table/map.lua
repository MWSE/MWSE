return {
	type = "function",
	description = [[Creates a new table consisting of key value pairs `k, f(k, v)`, where `k, v` is a pair in `t`.
Any additional arguments will be passed to `f`. For example, `table.map(t, f, 10)` would call `f(k, v, 10)` on each value `v` of `t`.]],
	arguments = {
		{ name = "t", type = "table" },
		{ name = "f", type = "fun(k: unknown, v: unknown, ...): unknown", description = "The function to apply to each element of `t`." },
		{ name = "...", type = "any", description = "Additional parameters to pass to `f`." },
	},
	returns = {
		{ name = "result", type = "table", description = "The result of applying `f` to each value in `t`." },
	}
}