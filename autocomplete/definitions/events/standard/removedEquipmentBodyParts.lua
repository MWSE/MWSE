return {
	type = "event",
	description = "This event is called prior to removing all body part nodes for worn equipment.",
	eventData = {
		["reference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "Whose body parts are being rebuilt.",
		},
	},
	filter = "reference",
}