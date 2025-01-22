return {
	type = "value",
	description = [[A copy of the list of mobiles with currently running AI. This does not include the player. The available objects are only valid at a point in time, and maybe be deleted or re-used any time a mobile is moved or disabled, so this data should only be used in the same moment that it is read. This is not a lightweight accessor, so it should be used carefully.]],
	readOnly = true,
	valuetype = "tes3mobileActor[]",
}