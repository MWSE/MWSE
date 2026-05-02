return {
	type = "value",
	description = [[Quick access to the base NPC's attributes.

!!! note
	This array is 1-indexed. The array indices correspond to the [`tes3.attribute`](https://mwse.github.io/MWSE/references/attributes/) table plus 1 to account for Lua's 1-based array indexing. In other words `npc.attributes[tes3.attribute.speed + 1]` returns the attribute value corresponding to Speed.]],
	valuetype = "number[]",
}
