return {
	type = "value",
	description = [[Hours added after `sunriseDuration` before the sunlight-color sunrise transition ends. The transition ends at `sunriseHour + sunriseDuration + sunPostSunriseTime`; the sunrise color is the midpoint, not the color at this field's value. This is an offset, not an absolute hour.]],
	valuetype = "number",
}