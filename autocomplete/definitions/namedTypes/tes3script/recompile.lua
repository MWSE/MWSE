return {
	type = "method",
	description = [[Replaces the bytecode of a script with the code compiled with the given mwscript. This should only be done during the initialized event, prior to a game being loaded.]],
	arguments = {
		{ name = "text", type = "string", optional = false, description = "The script text to compile. The line endings must be provided using CRLF." },
	},
	returns = {
		{ name = "success", type = "boolean", description = "If true, the script was recompiled successfully." },
	},
}