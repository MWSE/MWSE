return {
	type = "event",
	description = [[This event is fired when a dialogue with lua scripting is about to be executed. It allows third party code to inject themselves into the dialogue environment, so new functionality can be added more easily for dialogue scripters.

[This guide](https://mwse.github.io/MWSE/guides/lua-in-dialogues/) contains more info on using this event.]],
	eventData = {
		["environment"] = {
			type = "mwseDialogueEnvironment",
			readOnly = true,
			description = "The lua environment for the dialogue scripting. Any data added to it will be available as global variables to dialogue scripts.",
		},
	},
}
