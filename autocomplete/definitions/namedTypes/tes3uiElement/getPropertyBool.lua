return {
	type = "method",
	description = [[Properties are named variables attached to an element. Gets a property value with ``propName`` as the property key. Morrowind uses these to bind variables to the UI. Useful for element class-specific properties. Returns ``false`` if the property key did not have data.]],
	arguments = {
		{ name = "propName", type = "string", description = "The property name." },
	},
    valuetype = "boolean",
}