return {
	type = "method",
	description = [[Sets if this reference has active collision. This is preferable to the hasNoCollision property, if are manipulating multiple objects at once and do not want to constantly recalculate collision groups.]],
	arguments = {
		{ name = "hasNoCollision", type = "boolean", description = "If true, the reference no longer has collision." },
		{ name = "updateCollisions", type = "boolean", description = "If true, collision groups for the active cells are recalculated." },
	},
}