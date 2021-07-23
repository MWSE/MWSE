return {
	type = "function",
	description = [[Creates a new misc item object, which will be stored as part of the current saved game.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "id", type = "string", optional = true, description = "The new object's ID. Must be unique if provided." },
			{ name = "name", type = "string", default = "Miscellaneous item", description = "The new object's name." },
			{ name = "script", type = "tes3script", optional = true, description = "A script to attach to the object." },
			{ name = "mesh", type = "string", default = "", description = "The mesh to use for the object." },
			{ name = "objectFlags", type = "number", default = 0, description = "The object flags initially set. Force set as modified." },
			{ name = "weight ", type = "tes3script", default = 0, description = "The weight of the object." },
			{ name = "value ", type = "string", default = 0, description = "The value of the object." },
			{ name = "flags ", type = "number", default = 0, description = "The object flags initially set." }
		},
	}},
}
