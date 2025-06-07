return {
	type = "function",
	description = [[Plays a given animation group. Optional flags can be used to define how the group starts.

When 'group' is specified, the actor AI is paused while playing the animation, as if in an idle state. When one or more of 'lower', 'upper', or 'shield' are specified without 'group', the actor AI retains control, and the animations are layered. e.g. `tes3.playAnimation{reference = ..., upper = tes3.animationGroup.idle3, loopCount = 0}` will play the idle3 animation once (without looping) on the upper body, while the player or NPC is still able to walk around. Using 'upper' defers combat actions until the animation is done. Using 'shield' still allows combat actions.

As a special case, `tes3.playAnimation{reference = ..., group = 0}` returns control to the AI, as the AI knows that is the actor's neutral idle state.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3mobileActor|tes3reference|string", description = "The reference that will play the animation." },
			{ name = "group", type = "tes3.animationGroup", optional = true, description = "The animation group id to start playing -- a value from 0 to 149. Applies the animation to the whole body. Maps to [`tes3.animationGroup`](https://mwse.github.io/MWSE/references/animation-groups/) constants." },
			{ name = "lower", type = "tes3.animationGroup", optional = true, description = "Sets the animation group id for the lower body. This is used to combine different animations for each body section. Maps to [`tes3.animationGroup`](https://mwse.github.io/MWSE/references/animation-groups/) constants." },
			{ name = "upper", type = "tes3.animationGroup", optional = true, description = "Sets the animation group id for the upper body. This is used to combine different animations for each body section. Maps to [`tes3.animationGroup`](https://mwse.github.io/MWSE/references/animation-groups/) constants." },
			{ name = "shield", type = "tes3.animationGroup", optional = true, description = "Sets the animation group id for the shield arm. This is used to combine different animations for each body section. Maps to [`tes3.animationGroup`](https://mwse.github.io/MWSE/references/animation-groups/) constants." },
			{ name = "startFlag", type = "tes3.animationStartFlag", optional = true, default = "tes3.animationStartFlag.immediate", description = "A flag for starting the group with, using [`tes3.animationStartFlag`](https://mwse.github.io/MWSE/references/animation-start-flags/) constants." },
			{ name = "loopCount", type = "number", optional = true, default = -1, description = "If provided, the animation will repeat its loop section a given number of times. To make an animation play through once, set loopCount = 0. Defaults to infinite looping." },
			{ name = "mesh", type = "string", optional = true, description = "You can also use [`tes3.loadAnimation`](https://mwse.github.io/MWSE/apis/tes3/#tes3loadanimation) to reset loaded animations to default." },
		},
	}},
}
