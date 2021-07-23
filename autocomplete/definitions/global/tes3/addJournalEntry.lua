return {
	type = "function",
	description = [[This function creates a new journal entry. It can be called once the world controller is loaded.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "text", type = "string", description = "The text of the new Journal entry." },
        	{ name = "showMessage", type = "boolean", optional = true, default = true, description = "If this parameter is true, a \"Your journal has been updated\" message will be displayed." }
		},
	}},
}
