return {
	description = "The weatherTransitionFinished event occurs when the currently simulated weather finished transitioning to a new weather.",
	related = { "weatherCycled", "weatherTransitionFinished", "weatherChangedImmediate", "weatherTransitionStarted", "weatherChangedImmediate" },
	eventData = {
		to = {
			type = "tes3weather",
			description = "The weather object that will be transitioned to.",
		},
	},
}