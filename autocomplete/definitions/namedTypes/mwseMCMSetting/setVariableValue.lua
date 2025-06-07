return {
	type = "method",
	description = [[Changes the Setting's `variable.value` to the given value, updates the Setting's label and widget if needed, and calls `self:update`.]],
	arguments = {
		{ name = "newValue", type = "unknown" }
	}
}
