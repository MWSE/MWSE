return {
	type = "event",
	description =
	[[This event is triggered when a global script is started. This includes usage of the `StartScript` command, any scripts that are assigned as a "Start Script" in the editor, and any previously-running global scripts in the save file which get restarted after loading.

The primary use case of this event is to expose lua functionality to dialogue or morrowind script contexts.
	]],
	eventData = {
		["script"] = {
			type = "tes3script",
			readOnly = true,
			description = "The script that is being started.",
		},
		["reference"] = {
			type = "tes3reference|nil",
			readOnly = true,
			description = "The reference that the script is targeted at, if any.",
		},
	},
	examples = {
		["calculateCosine"] = {
			title = "Calculating cosine from mwscript",
		},
	},
	filter = "script.id",
	blockable = true,
}
