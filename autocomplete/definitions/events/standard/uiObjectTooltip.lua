return {
	description = "The uiObjectTooltip event triggers when a new tooltip is displayed for all in-world objects and items, and inventory tiles in all dialogues. The tooltip will be already be built. Tooltips for inventory tiles are built on mouseover, while tooltips for in-world objects are rebuilt every frame.",
	eventData = {
		["tooltip"] = {
			type = "tes3uiElement",
			readOnly = true,
			description = "The newly created tooltip element. Due to timeouts and target changes, it may be destroyed at any time.",
		},
		["object"] = {
			type = "tes3physicalObject",
			readOnly = true,
			description = "The object being examined.",
		},
		["reference"] = {
			type = "tes3reference",
			readOnly = true,
			description = "The reference being examined. Only valid for in-world objects. For inventory tiles it will always be nil.",
		},
		["itemData"] = {
			type = "tes3itemData",
			readOnly = true,
			description = "The item data of object, if any.",
		},
		["count"] = {
			type = "number",
			readOnly = true,
			description = "The number of items in the stack.",
		},
	},
}