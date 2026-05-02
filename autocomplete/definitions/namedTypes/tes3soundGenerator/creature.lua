return {
	type = "value",
	description = [[The base creature for this soundgen, which serves as a lookup index. Note that creatures can reference the soundgen of any other creature through the `tes3creature.soundCreature` property, which is generally used for variants of a creature.

When this is nil, the sound is treated as a generic fallback for this soundGen type, which is played when there is no creature match.]],
	valuetype = "tes3creature|nil",
}
