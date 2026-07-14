return {
	type = "event",
	description = [[
Fires at the end of each frame, right before rendering. All game logic and scene updates are complete, so this is the final opportunity to modify the scene before it's drawn.
	]],
	related = { "simulated", "enterFrame" },
}
