return {
	type = "method",
	description = [[Swaps the animations and related animation driven data for the two animation groups given. Groups are indexed by [`tes3.animationGroup`](https://mwse.github.io/MWSE/references/animation-groups/). It can be used on animation groups of the same class with matching actions, e.g. runForward1h and runForward2c. It will also work on animations that are currently playing, except attack animations. Changing an attack animation during an attack will permanently break the character controller until you change it back. Currently playing movement animations may be reset slightly to resynchronize the movement cycle.]],
	arguments = {
		{ name = "group1", type = "number", description = "An animation group to swap from [`tes3.animationGroup`](https://mwse.github.io/MWSE/references/animation-groups/)." },
		{ name = "group2", type = "number", description = "An animation group to swap from [`tes3.animationGroup`](https://mwse.github.io/MWSE/references/animation-groups/)." },
	}
}
