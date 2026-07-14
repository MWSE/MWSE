return {
	type = "value",
	description = [[Hours before `sunriseHour` when the ambient-color transition begins. The window is `sunriseHour - ambientPreSunriseTime` through `sunriseHour + sunriseDuration + ambientPostSunriseTime`, split between night → `ambientSunriseColor` and `ambientSunriseColor` → day. This is an offset, not an absolute hour.]],
	valuetype = "number",
}