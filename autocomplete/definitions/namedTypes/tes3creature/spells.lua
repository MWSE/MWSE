return {
	type = "value",
	description = [[Quick access to the base creature's spell list. It is a `tes3spellList`, which is a list wrapper with helper functions. The actual list is iterated over using `pairs`. E.g. `for _, spell in pairs(creature.spells) do print(spell.name) end`. It is not recommended to directly modify this list; instead use tes3.addSpell and tes3.removeSpell.]],
	readOnly = true,
	valuetype = "tes3spellList|tes3spell[]",
}