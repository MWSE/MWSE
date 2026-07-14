return {
	type = "value",
	description = [[Hours before `sunriseHour` when the fog-color transition begins. The window is `sunriseHour - fogPreSunriseTime` through `sunriseHour + sunriseDuration + fogPostSunriseTime`, split between night → `fogSunriseColor` and `fogSunriseColor` → day. This is an offset, not an absolute hour.]],
	valuetype = "number",
}