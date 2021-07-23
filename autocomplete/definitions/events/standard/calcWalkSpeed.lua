return {
	description = "One of the movement events, calcWalkSpeed is used when calculating the base walk speed.",
	eventData = {
		["mobile"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile actor whose speed is being calculated.",
		},
		["reference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "mobile’s related reference.",
		},
		["speed"] = {
			type = "number",
			description = "The previous speed calculated, starting with the base engine values.",
		},
		["type"] = {
			type = "number",
			readOnly = true,
			description = "The type of movement that was calculated. This is always 1 for this event.",
		},
	},
	links = {
        ["calcMoveSpeed"] = "lua/event/calcMoveSpeed",
        ["calcRunSpeed"] = "lua/event/calcRunSpeed",
        ["calcSwimSpeed"] = "lua/event/calcSwimSpeed",
        ["calcSwimRunSpeed"] = "lua/event/calcSwimRunSpeed",
    },
}