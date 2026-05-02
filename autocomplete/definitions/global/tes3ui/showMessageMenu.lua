return {
	type = "function",
	description = [[Displays a message box. This may be a simple toast-style message, or a box with choice buttons.]],
	arguments = {
		{
			name = "params",
			type = "table",
			tableParams = {
				{ name = "id", type = "string", optional = true, default = "MenuMessage", description = "The menu ID of the message menu." },
				{ name = "leaveMenuMode", type = "boolean", optional = true, default = true, description = "Determines if menu mode should be exited after a choice is made. By default it will exit menu mode, for backwards compatibility with existing mods." },
				{ name = "buttons", type = "tes3ui.showMessageMenu.params.button[]", description = "**Required** The list of buttons." },
				{ name = "callbackParams", type = "table", optional = true, description = "The table of parameters to pass to the callback functions." },
				{ name = "cancels", type = "boolean", optional = true, default = false, description = "When set to true, a cancel button is automatically added to the buttom of the list, even when paginated." },
				{ name = "cancelText", type = "string", optional = true, default = "tes3.findGMST(tes3.gmst.sCancel).value", description = "The text on the cancel button." },
				{ name = "cancelCallback", type = "function", optional = true, description = "The function to call when the user clicks the cancel button." },
				{ name = "header", type = "string|fun(): string", optional = true, description = "The optional header displayed above the message. Can also be a function that returns a string." },
				{ name = "message", type = "string|fun(): string", optional = true, description = "The message at the top of the messagebox. Can also be a function that returns a string." },
				{ name = "customBlock", type = "fun(parent: tes3uiElement)", optional = true, description = "A custom element to be displayed below the header. This function is passed a parent tes3uiElement, which it can modify to add a custom block according to your needs." },
				{ name = "page", type = "integer", optional = true, default = 1 },
				{ name = "pageSize", type = "integer", optional = true, default = 30 },
				{ name = "maxWidth", type = "integer", default = 400 },
			},
		},
	},
	returns = {
		{ name = "menu", type = "tes3uiElement" }
	},
}
