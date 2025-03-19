return {
	type = "method",
	description = [[This method is deprecated. Prefer to use `tes3uiElement.reorder` when moving single children.

Moves the layout order of the children of this element. `count` elements are taken from starting child `Element` or index (0-based) `moveFrom`, and moved before the child `Element` or index (0-based) `insertBefore`. If `count` is -1, all children after `moveFrom` are moved. If any index is a negative number, then the index represents a distance from the end of the child list.

Returns `true` if the operation succeeded, or `false` if at least one argument was invalid.]],
	arguments = {
		{ name = "insertBefore", type = "tes3uiElement|number", description = "The insertion point (or its 0-based child index)." },
		{ name = "moveFrom", type = "tes3uiElement|number", description = "The first child (or 0-based child index) to be moved." },
		{ name = "count", type = "number", description = "The number of child elements to move." },
	},
	valuetype = "boolean",
}