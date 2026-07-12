return {
	type = "value",
	description = [[Hours before `sunsetHour` when the ambient-color transition begins. The window is `sunsetHour - ambientPreSunsetTime` through `sunsetHour + sunsetDuration + ambientPostSunsetTime`, split between day → `ambientSunsetColor` and `ambientSunsetColor` → night. This is an offset, not an absolute hour.]],
	valuetype = "number",
}