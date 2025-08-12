return {
	type = "function",
	description = [[This function fetches a dictionary of the timings of the action keys for a specific animation group on an actor. The actor is required, as different actors can use different animations. The result is a table with action names as keys, and timings as values. The function will return nil if the actor does not have that animation group, or if the actor's animations are not active.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3reference|tes3mobileActor|string", description = "A reference to the which actor whose animations will be checked." },
			{ name = "group", type = "tes3.animationGroup|integer", description = "The animation group id to get the action timings for. Maps to [`tes3.animationGroup`](https://mwse.github.io/MWSE/references/animation-groups/) constants." },
		},
	}},
	returns = { name = "result", type = "table<string, number>|nil" }
}
