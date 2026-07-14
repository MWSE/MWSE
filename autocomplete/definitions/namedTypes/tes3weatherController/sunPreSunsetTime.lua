return {
	type = "value",
	description = [[Hours before `sunsetHour` when the sunlight-color transition begins. The window is `sunsetHour - sunPreSunsetTime` through `sunsetHour + sunsetDuration + sunPostSunsetTime`, split between day → `sunSunsetColor` and `sunSunsetColor` → night. This is an offset, not an absolute hour.]],
	valuetype = "number",
}