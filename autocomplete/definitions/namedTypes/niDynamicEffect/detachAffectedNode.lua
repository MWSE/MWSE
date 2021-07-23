return {
	type = "function",
	description = [[Removes a node from the dynamic effect's affected nodes list. The node's ``:updateEffects()`` function should be called afterwards to recognize the change.]],
	arguments = {
		{ name = "node", type = "niNode", description = "The node to remove from the affected nodes list." },
	},
}