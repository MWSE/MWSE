return {
	type = "method",
	description = [[Convenience function to get the relevant body part. Pass `isFemale` if the desired body part is female. If no female body part exists, the male bodyPart will be returned. This returns `nil` for invalid wearables.]],
	arguments = {
		{ name = "isFemale", type = "boolean", description = "If true, return the female part if it is valid. Otherwise, return the male part." },
	},
	valuetype = "tes3bodyPart?",
}