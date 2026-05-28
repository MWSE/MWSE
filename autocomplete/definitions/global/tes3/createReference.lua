return {
	type = "function",
	description = [[Similar to mwscript's PlaceAtPC or PlaceAtMe, this creates a new reference in the game world.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "object", type = "tes3physicalObject|string", description = "The object to create a reference of." },
			{ name = "position", type = "tes3vector3|number[]", description = "The location to create the reference at." },
			{ name = "orientation", type = "tes3vector3|number[]", description = "The new orientation for the created reference." },
			{ name = "cell", type = "tes3cell|string|table", optional = true, description = "The cell to create the reference in. This is only needed for interior cells." },
			{ name = "scale", type = "number", optional = true, default = 1.0, description = "A scale for the reference." },
			{ name = "updateCollisionGroups", type = "boolean", optional = true, default = true, description = "Only applies to activators, statics, containers, and lights that can't be carried. If false, collision will not be updated. Collision will need to be manually updated by calling `tes3.dataHandler:updateCollisionGroupsForActiveCells()`." },
		},
	}},
	returns = {{ name = "newReference", type = "tes3reference" }},
}
