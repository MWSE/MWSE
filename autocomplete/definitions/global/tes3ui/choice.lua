return {
	type = "function",
	description = [[Creates a simple dialogue choice, as per the `Choice` mwscript function. To add more options call this function multiple times.]],
	arguments = {
		{ name = "text", type = "string", description = "The text to display for the choice." },
		{ name = "index", type = "number", description = "The choice index associated with the given text." },
	},
}
