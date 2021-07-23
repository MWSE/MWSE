return {
	type = "method",
	description = [[Updates statistics derived from attributes, which are magicka, fatigue, and encumbrance. Will also update the UI if used on the player. Normally handled automatically when you use tes3.modStatistic.]],
	arguments = {
		{ name = "attribute", type = "tes3statistic", optional = true, description = "Limits the update to statistics derived from this attribute.  e.g. ``mobile:updateDerivedStatistics(mobile.strength)``. If not present, all derived statistics will be updated." },
	},
}