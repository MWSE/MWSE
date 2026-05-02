return {
	type = "value",
	description = [[The list of castable spells that the AI will consider using during combat. This includes both offensive and defensive spells. It is initialized at the start of combat and may be changed by mods at any time. Like other combat session data, it is not saved to a savegame.

This is actually a list collection type. You can use these methods on castableSpells:

castableSpells:add(tes3spell)
castableSpells:find(tes3spell) -> index
castableSpells:erase(index)
castableSpells:clear()

Note that ids cannot be used with these methods.
]],
	valuetype = "tes3spell[]",
}