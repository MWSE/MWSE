return {
	description = "This event fires when a crime is witnessed by an actor.",
	eventData = {
		["type"] = {
			type = "string",
			readOnly = true,
			description = "The type of crime that was committed.",
		},
		["position"] = {
			type = "tes3vector",
			readOnly = true,
			description = "The position that the crime ocurred at.",
		},
		["realTimestamp"] = {
			type = "number",
			readOnly = true,
			description = "The timestamp that the crime ocurred at.",
		},
		["value"] = {
			type = "number",
			readOnly = true,
			description = "The bounty value of the crime.",
		},
		["witness"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The reference that witnessed the crime.",
		},
		["witnessMobile"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile actor of the reference that witnessed the crime.",
		},
	},
	filter = "type",
}