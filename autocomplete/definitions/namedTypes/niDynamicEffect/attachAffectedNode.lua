return {
	type = "function",
	description = [[Adds a node to the dynamic effect's affected nodes list. The node's ``:updateEffects()`` function should be called afterwards to recognize the change.]],
	arguments = {
		{ name = "node", type = "niNode", description = "The node to add to the affected nodes list." },
	},
}