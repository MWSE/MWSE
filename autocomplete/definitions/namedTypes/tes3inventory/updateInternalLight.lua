return {
	type = "method",
	description = [[Re-evaluates whether the inventory contains a non-carriable internal light item and applies it to the given mobile actor if no active Light magic effect is present. This is useful when implementing custom light spells that can produce negative magnitudes or otherwise need vanilla-style fallback handling.]],
	arguments = {
		{ name = "mobile", type = "tes3mobileActor", description = "The mobile actor whose internal light state should be refreshed." }
	},
}
