return {
	type = "method",
	description = [[Attachs the child to the children list of the node.]],
	arguments = {
		{ name = "child", type = "niAVObject" },
		{ name = "useFirstAvailable", type = "bool", optional = true, description = "Use the first available space in the list. Default value is False." },
	},
}