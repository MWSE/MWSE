return {
	type = "function",
	description = [[This function is used to iterate over values and its subtables in the table.

Each "node" is an object with a children table of other "nodes", each of which might have their own children. For example, a sceneNode is made up of niNodes, and each niNodes can have a list of niNode children. This is best used for recursive data structures like UI elements and sceneNodes etc.]],
	arguments = {
		{ name = "t", type = "table", description = "A table to transverse." },
		{ name = "k", type = "unknown", optional = "true", default = "children", description = "The key of a table inside t object." },
	},
	valuetype = "iterator",
	examples = {
		["tableTransverse"] = {
			title = "Iterate over all scene nodes attached to player.",
			description = [[In the example below, function onLoaded() will be called when the game has been successfully loaded.

For each scene nodes attached to the player, its type (node.RTTI.name) and name (node.name), will be printed to MWSE.log.]],
		},
	},
}
