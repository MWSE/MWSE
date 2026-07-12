return {
	type = "value",
	description = [[Hours before `sunriseHour` when the sunlight-color transition begins. The window is `sunriseHour - sunPreSunriseTime` through `sunriseHour + sunriseDuration + sunPostSunriseTime`, split between night → `sunSunriseColor` and `sunSunriseColor` → day. This is an offset, not an absolute hour.]],
	valuetype = "number",
}