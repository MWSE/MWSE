return {
	type = "value",
	valuetype = "number",
	description = "A state index that indicates an actor's blocking state. It is zero when not blocking and non-zero when blocking. A value of 1 indicates a state transition from non-blocking to blocking, while a value of 2 means blocking is active (where the block animation is currently playing and should not be interrupted). The action simulation will reset the value to 0 at the end of a block animation.",
}