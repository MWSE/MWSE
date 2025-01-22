return {
	type = "value",
	description = [[An array-style table with access to the twenty seven NPC skill statistics.

!!! note
	This array is 1-indexed. The array indices correspond to the [tes3.skill](https://mwse.github.io/MWSE/references/skills/) table plus 1 to account for Lua's 1-based array indexing. In other words `myMobile.skills[tes3.skill.alchemy + 1]` returns the `tes3statisticSkill` object corresponding to alchemy skill.]],
	readOnly = true,
	valuetype = "tes3statisticSkill[]",
}
