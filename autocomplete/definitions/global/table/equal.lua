return {
	type = "function",
	description = [[Checks if one table is equal to another by recursively iterating through the (key, value) pairs of both tables.
Unlike the `==` operator, this will return `true` if two distinct tables have contents that compare equal.
For example, all of the following assertions pass:
```lua
assert(table.equal({1, 2}, {1, 2}))
assert({1, 2} ~= {1, 2})
assert(table.equal({a = 1, b = {x = 1}}, {a = 1, b = {x = 1}}))
```
]],
	arguments = {
		{ name = "left", type = "table" },
		{ name = "right", type = "table" },
	},
	returns = {
		{ name = "result", type = "boolean", description = "True if the contents of `left` are equal to the contents of `right`. False otherwise." },
	}
}