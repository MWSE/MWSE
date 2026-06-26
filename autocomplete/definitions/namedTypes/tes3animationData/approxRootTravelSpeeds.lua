return {
	type = "value",
	readOnly = true,
	description = [[The rounded root node travel speed for each of the animation groups, indexed by the [`tes3.animationGroup`](https://mwse.github.io/MWSE/references/animation-groups/) namespace. This is the base movement speed in units/sec of the animation. The speed may be 0, which means the animation rate does not scale with move speed. As this is a Lua array access, you will need to add one to the enum index.]],
	valuetype = "number[]",
}
