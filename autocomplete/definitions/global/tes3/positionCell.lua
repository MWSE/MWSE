return {
	type = "function",
	description = [[Positions a reference to another place.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3reference|tes3mobileActor|string", default = "tes3.mobilePlayer", description = "The reference to reposition." },
			{ name = "cell", type = "tes3cell", optional = true, description = "The cell to move the reference to. If not provided, the reference will be moved to a cell in the exterior worldspace at the position provided." },
			{ name = "position", type = "tes3vector3|table", description = "The position to move the reference to." },
			{ name = "orientation", type = "tes3vector3|table", optional = true, description = "The position to move the reference to."  },
			{ name = "forceCellChange", type = "boolean", default = false, description = "When true, forces the game to update a reference that has moved within a single cell, as if it was moved into a new cell." },
			{ name = "suppressFader", type = "boolean", default = false, description = "When moving the player, can be used to prevent the fade in and out visual effect." },
			{ name = "teleportCompanions", type = "boolean", default = true, description = "If used on the player, determines if companions should also be teleported." },
		},
	}},
	returns = {{ name = "executed", type = "boolean" }},
}
