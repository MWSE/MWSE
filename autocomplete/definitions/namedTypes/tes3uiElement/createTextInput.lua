return {
	type = "method",
	description = [[Creates a single line text input element. To receive input the keyboard must be captured with `tes3ui.acquireTextInput(element)`. Read the input with the `text` property. Write an initial value to display by setting the `text` property; that value will be cleared on the first keypress.

Text input specific properties can be accessed through the `widget` property. The widget type for text inputs is [`tes3uiTextInput`](https://mwse.github.io/MWSE/types/tes3uiTextInput/).]],
	arguments = { {
		name = "params",
		type = "table",
		optional = true,
		tableParams = {
			{ name = "id", type = "string|number", optional = true, description = "An identifier to help find this element later." },
			{ name = "text", type = "string", optional = true, description = "The initial text to use for the input." },
			{ name = "placeholderText", type = "string", optional = true, description = "Placeholder text for the input. If the element is ever made empty, this will be displayed instead in the disabled text color." },
			{ name = "numeric", type = "boolean", optional = true, default = false, description = "If true, only numbers can be put into the input. The text value of the element will still be a string, and need to be converted using `tonumber`." },
			{ name = "autoFocus", type = "boolean", optional = true, default = false, description = "If true, the input will be automatically focused after creation." },
			{ name = "createBorder", type = "boolean", optional = true, default = false, description = "If true, a thin border will be created around the input box. By default it will have standard padding, and will have `widthProportional` set to `1.0`. It can be accessed by the return value's `.parent`." },
		},
	} },
	valuetype = "tes3uiElement",
	examples = {
		["SearchBox"] = {
			title = "Search box",
			description = "Text input is just made of text, it doesn't have a border. It's often desirable, to make it clearer to the user, to put the text input inside a thin border.",
		}
	}
}
