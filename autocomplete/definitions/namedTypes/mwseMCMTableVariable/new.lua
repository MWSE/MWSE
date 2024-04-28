return {
	type = "method",
	description = [[Creates a new variable of this type.]],
	arguments = {{
		name = "variable",
		type = "table",
		tableParams = {
			{ name = "id", type = "string|number", description = "Key in the config file used to store the variable." },
			{ name = "table", type = "table", description = "The table to save the data to." },
			{ name = "defaultSetting", type = "unknown", optional = true, description = "If `id` does not exist in the table, it will be initialised to this value." },
			{ name = "inGameOnly", type = "boolean", optional = true, default = false, description = "If true, the setting containing this variable will be disabled if the game is on main menu." },
			{ name = "restartRequired", type = "boolean", optional = true, default = false, description = "If true, updating the setting containing this variable will notify the player to restart the game." },
			{ name = "restartRequiredMessage", type = "string", optional = true, description = " The default text is a localized version of: \"The game must be restarted before this change will come into effect.\"." },
			{ name = "converter", type = "fun(newValue): unknown", optional = true, description = "This function is called when the value of the variable is changed. The function can modify the new value before it is saved." },
		}
	}},
	returns = {{
		name = "variable", type = "mwseMCMTableVariable"
	}}
}
