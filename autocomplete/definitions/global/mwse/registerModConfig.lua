return {
	type = "function",
	description = [[This is the main function to register a mod's configuration. Only registered configurations appear in the Mod Config menu.]],
	arguments = {
		{ name = "name", type = "string" },
		{
			name = "package",
			type = "table",
			tableParams = {
				{ name = "onCreate", type = "fun(modConfigContainer: tes3uiElement)", description = "The function that creates the mod's configuration menu inside given `modConfigContainer`." },
				{ name = "onSearch", type = "fun(searchText: string): boolean", optional = true, description = "A custom search handler function. This function should return true if this mod should show up in search results for given `searchText`." },
				{ name = "onClose", type = "fun(modConfigContainer: tes3uiElement)", optional = true, description = "This function is called when the mod's configuration menu is closed. Typically, it's used to save the current config table." },
				{ name = "template", type = "mwseMCMTemplate", optional = true, description = "Used internally by `Template:register()`." },
			}
		},
	},
}
