return {
	type = "function",
	description = [[This function performs a DFS over a graph-like table. You can specify the key of the subtable that contains the child nodes. A filter can be provided to skip a certain part of the tree.

Each "node" is an object with a children table of other "nodes", each of which might have their own children. For example, a sceneNode is made up of niNodes, and each niNodes can have a list of niNode children. This is best used for recursive data structures like UI elements and sceneNodes etc.]],
	generics = {
		{ name = "tableType" },
	},
	arguments = {
		{ name = "t", type = "tableType", description = "A table to transverse." },
		{ name = "k", type = "string|table.traverse.filter", optional = true, default = "children", description = "The subtable key. The function is overloaded, so you can pass the filter as the second argument." },
		{ name = "filter", type = "fun(node: niAVObject|unknown): boolean, boolean", description = "The filter function. It gets passed each node before it's yielded by the iterator. It has to return 2 values: `skipThisNode` and `skipChildren`. If `skipThisNode` is returned as `true`, then this node and its children will be skipped. If `skipThisNode` is `false` and `skipChildren` is `true` then this node will be iterated over but the child nodes will not." },
	},
	returns = {
		name = "iterator",
		type = "fun(): tableType|any",
	},
	examples = {
		["tableTransverse"] = {
			title = "Iterate over all scene nodes attached to player.",
			description = [[In the example below, function onLoaded() will be called when the game has been successfully loaded.

For each scene nodes attached to the player, its type (node.RTTI.name) and name (node.name), will be printed to MWSE.log.]],
		},
		["Filter collision geometry"] = {
			title = "Iterate over scene graph skipping any collision geometry",
			description = [[In this example, a filter function is used to skip over the `RootCollisionNode`s that hold geometry used for collision.]],
		}
	},
}
