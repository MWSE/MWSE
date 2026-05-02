return {
	type = "value",
	readOnly = true,
	description = [[An array-style table with access to the reference's attach node objects.

!!! note
	This array is 1-indexed. The array indices correspond to the [`tes3.bodyPartAttachment`](https://mwse.github.io/MWSE/references/body-part-attachments/) table plus 1 to account for Lua's 1-based array indexing. In other words `bodyPartManager.attachNodes[tes3.bodyPartAttachment.leftHand + 1]` returns the `tes3bodyPartManagerAttachNode` object corresponding to the left hand.

	Prefer using [`getAttachNode`](https://mwse.github.io/MWSE/types/tes3bodyPartManager/#getattachnode) instead to avoid confusion with the 1-based indexing.
]],
	valuetype = "tes3bodyPartManagerAttachNode[]",
}
