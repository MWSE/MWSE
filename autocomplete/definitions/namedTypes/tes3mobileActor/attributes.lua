return {
	type = "value",
	description = [[Access to a table of 8 [`tes3statistic`](https://mwse.github.io/MWSE/types/tes3statistic/) objects for the actor's attributes. If you are setting player stats, instead use `tes3.setStatistic` to also update the UI immediately.

!!! note
	This array is 1-indexed. The array indices correspond to the [tes3.attribute](https://mwse.github.io/MWSE/references/attributes/) table plus 1 to account for Lua's 1-based array indexing. In other words `myMobile.attributes[tes3.attribute.strength + 1]` returns the `tes3statistic` object corresponding to strength.]],
	readOnly = true,
	valuetype = "tes3statistic[]",
}
