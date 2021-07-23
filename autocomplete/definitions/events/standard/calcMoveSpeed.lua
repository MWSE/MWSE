return {
	description = "This event is called when an actor’s movement speed is calculated. The event allows modification of this value, to dehardcode actor movement speeds. Invoked after all other movement speed events are finished.",
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
			description = "The type of movement that was calculated. This is always 0 for this event.",
		},
	},
	examples = {
		["FastMovement"] = {
			title = "20% Faster Player Movement",
		},
	},
	links = {
        ["calcWalkSpeed"] = "lua/event/calcWalkSpeed",
        ["calcRunSpeed"] = "lua/event/calcRunSpeed",
        ["calcSwimSpeed"] = "lua/event/calcSwimSpeed",
        ["calcSwimRunSpeed"] = "lua/event/calcSwimRunSpeed",
        ["calcFlySpeed"] = "lua/event/calcFlySpeed",
    },
}