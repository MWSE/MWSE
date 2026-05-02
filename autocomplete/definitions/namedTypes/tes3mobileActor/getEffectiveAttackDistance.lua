return {
	type = "method",
	description = "Returns the distance used for checking attack range. This is measured by the distance between the actors' bounding boxes edges, as if the actors were exactly facing each other. The number may be negative if the bounding boxes overlap.",
	arguments = {
		{ name = "mobile", type = "tes3mobileActor", description = "The target actor." },
	},
	returns = { name = "distance", type = "number" }
}