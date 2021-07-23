return {
	type = "function",
	description = [[Create an object and returns it. The created object will be part of the saved game. Supported object types are those that have their own create function, such as tes3activator for example.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "objectType", type = "number", optional = false, description = "Maps to tes3.objectType constants. Used to filter object type to create." }
		},
	}},
	returns = "table",
	valuetype = "tes3BaseObject",
	examples = {
		["createMisc"] = {
			title = "Creates a tes3misc object",
			description = "The example below create a misc item object that could be used to create a placeable reference later on."
		},
		["createStatic"] = {
			title = "Creates a tes3static object",
			description = "The example below create a static object that could be used to create a placeable reference later on."
		},
	},
}
