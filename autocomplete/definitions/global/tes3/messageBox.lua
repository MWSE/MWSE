return {
	type = "function",
	description = [[Displays a message box. This may be a simple toast-style message, or a box with choice buttons.]],
	arguments = {
		{
			name = "messageOrParams",
			type = "string|table",
			tableParams = {
				{ name = "message", type = "string" },
				{ name = "buttons", type = "table", description = "An array of strings to use for buttons.", optional = true },
				{ name = "callback", type = "function" },
				{ name = "showInDialog", type = "boolean", description = "Specifying showInDialog = false forces the toast-style message, which is not shown in the dialog menu. Defaults to true.", optional = true },
				{ name = "duration", type = "float", description = "Overrides how long the toast-style message remains visible.", optional = true },
			},
		},
		{ name = "formatAdditions", type = "variadic", optional = true, description = "Only used if messageOrParams is a string." },
	},
	returns = {
		{ name = "element", type = "tes3uiElement|nil", description = "The UI menu created for the notification, if any." }
	}
}
