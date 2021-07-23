return {
	description = "This event is invoked whenever something is activated, typically by the player. Activation is usually done with the associated activate/use key, but may also be forced by scripts.",
	eventData = {
		["activator"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The actor attempting to trigger the event.",
		},
		["target"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The reference that is being activated.",
		},
	},
	filter = "target",
	examples = {
		["ShowActivationTarget"] = {
			title = "Show a message for what the player is activating",
		},
		["PreventActivatingNPCs"] = {
			title = "Prevent the player from activating NPCs",
		},
	},
	links = {
		["xActivate"] = "mwscript/functions/actor/xActivate",
	},
}