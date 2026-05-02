return {
	type = "method",
	description = [[Properties are extra variables attached to an element. Morrowind uses these to bind variables to the UI, and they can be useful for element class-specific properties. This function gets a property as an object value, defaulting to `nil` if the property was not set. This function can be dangerous to use, and can lead to crashes if not properly understood.]],
	arguments = {
		{ name = "property", type = "number|string", description = "The property to get." },
		{ name = "typeCast", optional = true, default = "\"tes3baseObject\"", type = "string", description = "The casting of the property to get." },
	},
	returns = {
		{ name = "object", type = "any", description = "The value of the property, defaulting to `nil` if the property was not set." },
	},
}
