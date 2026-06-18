return {
	type = "function",
	description = [[Returns a proxy that guards normal assignment through the returned table.

This protects normal assignment on the proxy or converted tables, but it is not a sandbox boundary against `rawset`, debug-library metatable or upvalue access, or hostile privileged code. Omitting `copy` converts the input table in place and rehomes raw entries behind backing storage.]],
	generics = {
		{ name = "tableType", inherits = "table" },
	},
	arguments = {
		{ name = "inTable", type = "tableType" },
		{ name = "copy", type = "boolean", optional = true, default = false, description = "Copy instead of converting in place." },
		{ name = "strict", type = "boolean", optional = true, description = "Throw when reading missing keys." },
	},
	returns = {
		{ name = "result", type = "table", description = "The readonly table proxy." },
	},
}
