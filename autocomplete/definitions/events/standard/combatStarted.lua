return {
	type = "event",
	description = [[The **combatStarted** event occurs after combat has began between two actors. This event is preceded by the [combatStart](https://mwse.github.io/MWSE/events/combatStart) event. An actor A can start combat with actor B, which will fire the event. Then the actor B may also start combat with actor A which will trigger the event again.]],
	related = { "combatStart", "combatStarted", "combatStop", "combatStopped" },
	eventData = {
		["actor"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile actor who has entered combat.",
		},
		["target"] = {
			type = "tes3mobileActor",
			readOnly = true,
			description = "The mobile actor who combat has been triggered against.",
		},
	},
}