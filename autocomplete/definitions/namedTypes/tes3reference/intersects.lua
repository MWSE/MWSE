return {
	type = "method",
	arguments = {
		{ name = "other", type = "tes3reference", description = "The other reference to check.", }
	},
	description = [[Checks if the bounding box of another reference intersects the bounding box of this reference. This can be used to see if objects are "too close together".]],
	valuetype = "boolean",
	examples = {
		["isPlayerTooClose"] = {
			title = "Check if the player is too close to another reference.",
			description = "The example below illustrates how you might detect whether the player is too close to a given object. The passed `reference` could be to a container, prop, NPC, etc."
		},
	},
}
