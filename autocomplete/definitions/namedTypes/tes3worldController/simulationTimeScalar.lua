return {
	type = "value",
	description = [[A scalar used for simulation time. At the start of every frame, the `deltaTime` is multiplied by this value. Doing this here is safer than doing it in another event. This value doesn't need to be modified every frame. You need to restore it to its original value to cancel the time scaling.]],
	valuetype = "number",
}