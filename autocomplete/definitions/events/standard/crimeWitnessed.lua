return {
	type = "event",
	description = "This event fires when a crime is witnessed by an actor.",
	eventData = {
		["type"] = {
			type = "string",
			readOnly = true,
			description = "The type of crime that was committed. The type can be \"attack\", \"killing\", \"stealing\", \"pickpocket\", \"theft\", \"trespass\", and \"werewolf\". Crime \"theft\" is raised when picking up owned items. Crime \"trespass\" is raised when lockpicking, probing, using Open magic on owned doors or chests and sleeping in owned beds."
		},
		["position"] = {
			type = "tes3vector3",
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
			description = "The total stolen items value of the crime. Only valid for thefts. See the example below to calculate the bounty incurred on each crime type.",
		},
		["victim"] = {
			type = "tes3actor|tes3faction",
			readOnly = true,
			description = "The victim of the crime, as a general actor base object or faction.",
		},
		["victimFaction"] = {
			type = "tes3faction",
			readOnly = true,
			description = "The faction that the crime was against, assuming the victim is, or is in, one.",
		},
		["victimMobile"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile of the victim, if applicable, giving access to the unique victim.",
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
		["crimeEvent"] = {
			type = "tes3crimeEvent",
			readOnly = true,
			description = "Direct access to the crime event data that was witnessed.",
		},
	},
	filter = "type",
	examples = {
		["bountyValue"] = {
			title = "Getting bounty value after certain crime type",
		}
	}
}
