return {
	type = "method",
	description = [[Fetches the current value of a skill with a given index. This is the way to access skills for any type of actor, as creatures have a limited version of the skill system. Note that creatures share a statistic between multiple skills (they only have combat, magic, and stealth stats), so many values will be the same.]],
	arguments = {
		{ name = "skillId", type = "number", description = "The index of the skill statistic's value to fetch." },
	},
	valuetype = "number",
}