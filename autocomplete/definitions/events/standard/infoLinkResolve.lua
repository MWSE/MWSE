return {
	description = "This event fires when a dialogue hyperlink is being resolved to a topic, during UI layout (not on link activation).",
	related = { "infoGetText", "infoLinkResolve", "infoResponse", "infoFilter", "postInfoResponse" },
	eventData = {
		["topic"] = {
			type = "string",
			description = "The topic that activates when this hyperlink is clicked. Initially set to the link text. Set this value to change the target topic.",
		},
	},
}