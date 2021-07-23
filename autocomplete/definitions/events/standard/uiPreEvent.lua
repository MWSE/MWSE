return {
	description = "uiPreEvent is triggered through various UI events. This includes scrolling through panes, clicking buttons, selecting icons, or a host of other UI-related activities. This event fires before uiEvent, and has the additional advantage of being able to be blocked.",
	eventData = {
		["block"] = {
			type = "tes3uiElement",
			readOnly = true,
			description = "The UI element that is firing this event.",
		},
		["parent"] = {
			type = "tes3uiElement",
			readOnly = true,
			description = "The calling element's parent.",
		},
		["property"] = {
			type = "number",
			readOnly = true,
			description = "The property identifier that is being triggered.",
		},
		["var1"] = {
			type = "number",
			readOnly = true,
			description = "One of two undefined variables related to the event.",
		},
		["var2"] = {
			type = "number",
			readOnly = true,
			description = "One of two undefined variables related to the event.",
		},
	},
}