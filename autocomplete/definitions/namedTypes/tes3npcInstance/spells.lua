return {
	type = "value",
	description = [[Quick access to the base NPC's spell list. It is a tes3spellList, which is a list wrapper with helper functions. The actual list is accessed with .iterator. e.g. for _, spell in pairs(npc.spells.iterator) do print(spell.name) end]],
	readOnly = true,
	valuetype = "tes3spellList",
}