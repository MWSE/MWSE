return {
	type = "event",
	description = "This event fires when an actor has determined an action in a combat session. The action chosen is `session.selectedAction`, which can be changed to affect the actor AI. Note that other data in `e.session` like `selectedWeapon` or `selectedMagic` is only indicative of the AI's choice. The weapon or magic that the actor uses must be set on the mobile.",
	related = { "determineAction" },
	eventData = {
		["session"] = {
			type = "tes3combatSession",
			readOnly = true,
			description = "The combat session the action has been determined for.",
		},
	},
}