return {
	type = "function",
	description = [[Searches for active mobile actors which are within ``range`` distance from the ``reference`` or ``position`` argument. It only finds mobiles which have active AI, and can include the player. This function is used by the game for area-of-effect hits. It has a small amount of overhead, so try not to use it too much.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3reference|tes3mobileActor|string", optional = true, description = "The position to search from, taken from a reference. Uses the position of the centre of the body if the reference is an actor." },
			{ name = "position", type = "tes3vector3", optional = true, description = "The position to search from." },
			{ name = "range", type = "number", description = "The distance around the position to search. An actor is included if the centre of their body is in range." },
		},
	}},
	returns = {{ name = "mobileList", type = "table" }},
}