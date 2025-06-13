return {
	type = "function",
	description = [[Creates a new table consisting of key value pairs `k, f(k, v)`, where `k, v` is a pair in `t`.
Any additional arguments will be passed to `f`. For example, `table.map(t, f, 10)` would call `f(k, v, 10)` on each value `v` of `t`.]],
	generics = {
		{ name = "keyType" },
		{ name = "valueType" },
		{ name = "newValueType" },
	},
	arguments = {
		{ name = "t", type = "{ [keyType]: valueType }" },
		{ name = "f", type = "fun(k: keyType, v: valueType, ...):newValueType", description = "The function to apply to each element of `t`." },
		{ name = "...", type = "any", description = "Additional parameters to pass to `f`." },
	},
	returns = {
		{ name = "result", type = "{ [keyType]: newValueType }", description = "The result of applying `f` to each value in `t`." },
	}
}