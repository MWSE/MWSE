return {
	type = "value",
	description = [[Hours before `sunsetHour` when the fog-color transition begins. The window is `sunsetHour - fogPreSunsetTime` through `sunsetHour + sunsetDuration + fogPostSunsetTime`, split between day → `fogSunsetColor` and `fogSunsetColor` → night. This is an offset, not an absolute hour.]],
	valuetype = "number",
}