return {
	type = "value",
	description = [[Absolute game hour when the stars start fading out. At construction, this is initialized as `sunriseHour -` the `Morrowind.ini` offset (default offset `2` hours); the field itself is then compared directly with the current game hour. Despite its name, it marks the start of the fade-out.]],
	valuetype = "number",
}