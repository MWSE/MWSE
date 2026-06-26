return {
	type = "method",
	description = [[This method plays an animation group on the related actor, invoking the `playGroup` event.]],
	arguments = {
		{ name = "animationGroup", type = "tes3.animationGroup", description = "The animation group to play. A value from [`tes3.animationGroup`](https://mwse.github.io/MWSE/references/animation-groups/) namespace." },
		{ name = "startFlag", type = "tes3.animationStartFlag", description = "A flag for starting the group with, using [`tes3.animationStartFlag`](https://mwse.github.io/MWSE/references/animation-start-flags/) constants." },
		{ name = "loopCount", type = "number", description = "If provided, the animation will repeat its loop section a given number of times. To make an animation play through once, set loopCount = 0, while -1 is used for infinite looping." }
	}
}
