return {
	description = "The isGuard event triggers whenever the game engine checks if an NPC object is a guard. The guard status can be modified. This alone will not make an NPC behave like a guard, though.",
	eventData = {
		npc = {
			type = "tes3npc",
			readOnly = true,
			description = "The NPC that the guard status is being checked for.",
		},
		isGuard = {
			type = "bool",
			readonly = false,
			description = "The previously calculated guard status.",
		},
	},
}