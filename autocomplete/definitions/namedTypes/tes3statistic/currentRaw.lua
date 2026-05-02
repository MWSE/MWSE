return {
	type = "value",
	description = [[The current value of the statistic, as stored in memory. When a certain statistic would reach negative value, its `current` value will be `0` while `currentRaw` will be the actual negative value. Prefer to use `.current` unless you know what you're doing.]],
	valuetype = "number",
}