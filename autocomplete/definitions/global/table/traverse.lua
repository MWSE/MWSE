return {
	type = "function",
	description = [[This function is used to iterate over a graph-like table. You can specify the key of the subtable that contains the child nodes.

Each "node" is an object with a children table of other "nodes", each of which might have their own children. For example, a sceneNode is made up of niNodes, and each niNodes can have a list of niNode children. This is best used for recursive data structures like UI elements and sceneNodes etc.]],
	generics = {
		{ name = "tableType" },
	},
	arguments = {
		{ name = "t", type = "tableType", description = "A table to transverse." },
		{ name = "k", type = "string", optional = true, default = "children", description = "The subtable key." },
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
	},
}
