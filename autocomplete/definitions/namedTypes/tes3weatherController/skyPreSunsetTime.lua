return {
	type = "value",
	description = [[Hours before `sunsetHour` when the sky-color transition begins. The window is `sunsetHour - skyPreSunsetTime` through `sunsetHour + sunsetDuration + skyPostSunsetTime`, split between day → `skySunsetColor` and `skySunsetColor` → night. This is an offset, not an absolute hour.]],
	valuetype = "number",
}