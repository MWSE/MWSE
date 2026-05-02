return {
	type = "value",
	description = [[The sound played when an item is repaired.

!!! bug
	Due to a bug in the game engine, the initialization code for this field never sets it to any `tes3sound` object. Instead, use sound IDs "repair" or "repair fail."]],
	valuetype = "tes3sound|nil",
}