return {
	type = "function",
	description = [[Returns a safe handle for the object. To get the object use `:getObject()`. To check if it still exists use `:valid()`.]],
	arguments = {
		{ name = "object", type = "tes3baseObject", description = "An object to make a safe handle for." }
	},
	returns = { name = "safeObjectHandle", type = "mwseSafeObjectHandle" },
	examples = {
		["..\\rayTest\\rayTestForLater"] = {
			title = "Example"
		}
	}
}
