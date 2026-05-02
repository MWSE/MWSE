return {
	type = "value",
	description = [[A table of twenty seven numbers, representing the base values for the NPC's skills.

!!! note
	This array is 1-indexed. The array indices correspond to the [`tes3.skill`](https://mwse.github.io/MWSE/references/skills/) table plus 1 to account for Lua's 1-based array indexing. In other words `npc.skills[tes3.skill.armorer + 1]` returns the skill value corresponding to the Armorer skill.]],
	valuetype = "number[]",
}
