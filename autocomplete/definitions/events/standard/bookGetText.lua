return {
	description = "This event is raised when a book's text is about to be displayed. By providing your own text, you can overwrite what is going to be displayed.",
	eventData = {
		["book"] = {
			type = "tes3book",
			readOnly = true,
			description = "The book whose text is being retrieved.",
		},
		["text"] = {
			type = "string",
			description = "If set, the book's text will be overwritten with this value.",
		},
	},
	filter = "book",
}