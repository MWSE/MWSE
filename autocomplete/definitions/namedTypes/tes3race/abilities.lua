return {
	type = "value",
	description = [[A list of abilities that NPCs of that race have access to. It is a tes3spellList, which is a list wrapper with helper functions. The actual list is accessed with .iterator. e.g. for _, ability in pairs(race.abilities.iterator) do print(ability.name) end]],
	readOnly = true,
	valuetype = "tes3spellList",
}