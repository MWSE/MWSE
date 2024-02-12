return {
	type = "function",
	description = [[Displays a message box. This may be a simple toast-style message, or a box with choice buttons.]],
	arguments = {
		{
			name = "messageOrParams",
			type = "boolean|number|string|table",
			tableParams = {
				{ name = "message", type = "string" },
				{ name = "buttons", type = "string[]", description = "An array of strings to use for buttons. Maximal text length on each button is 32 characters.", optional = true },
				{ name = "callback", type = "fun(e: tes3messageBoxCallbackData)", optional = true, description = "The callback function will be executed after a button was pressed. The callback function will be passed a table with `button` field corresponding to 0-based index of the button from passed `buttons` array." },
				{ name = "showInDialog", type = "boolean", optional = true, default = "true", description = "Specifying showInDialog = false forces the toast-style message, which is not shown in the dialog menu." },
				{ name = "duration", type = "number", description = "Overrides how long the toast-style message remains visible.", optional = true },
			},
		},
		{ name = "...", type = "any", optional = true, description = "Formatting arguments. These are passed to `string.format`, provided `messageOrParams` is a `string`." },
	},
	returns = {
		{ name = "element", type = "tes3uiElement|nil", description = "The UI menu created for the notification, if any." }
	},
	examples = {
		["callbackExample"] = {
			title = "A message box with a callback function",
			description = "This demonstrates how to determine which button was pressed inside callback function.",
		}
	}
}
