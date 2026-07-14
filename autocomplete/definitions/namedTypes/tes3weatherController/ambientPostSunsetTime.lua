return {
	type = "value",
	description = [[Hours added after `sunsetDuration` before the ambient sunset transition ends. The transition ends at `sunsetHour + sunsetDuration + ambientPostSunsetTime`; the sunset color is the midpoint, not the color at this field's value. This is an offset, not an absolute hour.]],
	valuetype = "number",
}