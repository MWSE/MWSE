return {
	type = "function",
	description = [[Recursively compares tables by key/value equality without recursing forever on cycles.

Tables are structurally compared with raw table identity checks. Table `__eq` metamethods are not invoked for table identity or table values. Non-table values use normal Lua equality.]],
	arguments = {
		{ name = "left", type = "any" },
		{ name = "right", type = "any" },
	},
	valuetype = "boolean",
}
