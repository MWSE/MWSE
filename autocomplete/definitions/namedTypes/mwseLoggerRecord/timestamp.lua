return {
	type = "value",
	description = "The timestamp associated to this record. This is obtained directly from `socket.gettime()`. \z
		In particular, it captures the current real-world time, rather than the amount of time since the game launched.\n\z
		\n\z
		Will be `false` when `Logger.includeTimestamp` is `false`.",
	valuetype = "number|false",
}
