return {
	type = "function",
	description = [[Checks if a merchant will offer a service to you, including dialogue checks like disposition and faction membership. A specific service can be checked, or if no service is given, a generic dialogue check is made. If the service if refused, the dialogue reply for the refusal may also returned (it may be nil, as there may not always be a reply available).]],
	arguments = {
		{ name = "reference", type = "tes3reference|tes3mobileActor|string" },
		{ name = "service", type = "number", optional = true, description = "The specific service to check for availability. Uses the tes3.merchantService.* constants." },
	},
	returns = "offersService, refusalReply",
	valuetype = "boolean, tes3DialogueInfo",
}