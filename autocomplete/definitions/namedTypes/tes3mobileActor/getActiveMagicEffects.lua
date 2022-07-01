return {
	type = "method",
	description = [[Fetches a filtered list of the active magic effects on the actor. Returns a table with [`tes3activeMagicEffect`](https://mwse.github.io/MWSE/types/tes3activeMagicEffect/) items.]],
	arguments = {{
		name = "params",
		type = "table",
		optional = true,
		tableParams = {
			{ name = "effect", type = "number", optional = true, description = "The magic effect ID to search for." },
			{ name = "serial", type = "number", optional = true, description = "The magic instance serial to search for." },
		},
	}},
	valuetype = "tes3activeMagicEffect[]",
}
