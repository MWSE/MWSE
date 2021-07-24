return {
	description = "The weatherChangedImmediate event occurs when the currently simulated weather is changed without transition.",
	related = { "weatherCycled", "weatherTransitionFinished", "weatherChangedImmediate", "weatherTransitionStarted", "weatherChangedImmediate" },
	eventData = {
		to = {
			type = "tes3weather",
			description = "The weather object that is the new current weather.",
		},
	},
}