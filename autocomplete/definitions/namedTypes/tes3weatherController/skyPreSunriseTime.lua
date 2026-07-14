return {
	type = "value",
	description = [[Hours before `sunriseHour` when the sky-color transition begins. The window is `sunriseHour - skyPreSunriseTime` through `sunriseHour + sunriseDuration + skyPostSunriseTime`, split between night → `skySunriseColor` and `skySunriseColor` → day. This is an offset, not an absolute hour.]],
	valuetype = "number",
}