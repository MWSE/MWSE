
return {
	type = "value",
	valuetype = "number",
	description = "A 16-bit hourstamp of the last time a merchant's barter gold was at its base level. It is used as a timer for resetting barter gold, which happens on interacting with a merchant at hourstamp `lastBarterHoursPassed + GMST fBarterGoldResetDelay` or later. Barter gold can also reset when a mobile expires.",
}