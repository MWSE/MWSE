return {
	type = "method",
	description = [[Performs a DFS walk over the node's child tree.]],
	arguments = {{
		name = "args",
		optional = true,
		type = "table",
		tableParams = {
			{ name = "type", type = "ni.type|ni.type[]|integer", optional = true, description = "If provided, only NI objects of provided type are yielded. Can be a single type, or an array of multiple types." },
			{ name = "prefix", type = "string", optional = true, description = "If provided, only NI objects with a name matching the prefix are yielded." },
			{ name = "recursive", type = "boolean", optional = true, default = true, description = "If true, the method also walk over the nested nodes." }
		}
	}},
	returns = {{
		name = "iterator",
		type = "fun(): niAVObject"
	}},
	examples = {
		["Usage"] = {}
	}
}
