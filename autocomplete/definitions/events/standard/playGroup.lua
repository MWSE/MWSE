return {
	type = "event",
	description = "This event is triggered when an animation is about to be played. This happens after the `simulated` event.",
	eventData = {
		["animationData"] = {
			type = "tes3animationData",
			readOnly = true,
			description = "The related animation data.",
		},
		["reference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The actor that is about to play the animation.",
		},
		["currentGroup"] = {
			type = "tes3.animationGroup",
			description = "The animation group that is currently playing. Maps to values in [`tes3.animationGroup`](https://mwse.github.io/MWSE/references/animation-groups/) namespace.",
		},
		["group"] = {
			type = "tes3.animationGroup",
			description = "The animation group about to be played. Maps to values in [`tes3.animationGroup`](https://mwse.github.io/MWSE/references/animation-groups/) namespace.",
		},
		["index"] = {
			type = "tes3.animationBodySection",
			readOnly = true,
			description = "The index of the body section the animation will be played on. Maps to values in [`tes3.animationBodySection`](https://mwse.github.io/MWSE/references/animation-body-sections/) namespace.",
		},
		["flags"] = {
			type = "tes3.animationStartFlag",
			description = "Maps to values in [`tes3.animationStartFlag`](https://mwse.github.io/MWSE/references/animation-start-flags/) namespace.",
		},
		["loopCount"] = {
			type = "number",
			description = "The number of times the animation is going to be played. Infinite looping is marked by `-1`, while `0` means the animation will be played only once (no looping).",
		},
	},
	filter = "reference",
	blockable = true,
}
