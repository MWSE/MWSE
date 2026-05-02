return {
	type = "method",
	description = [[Performs vanilla armor summoning logic, but also allows bracers and pauldrons. It can summon one or two armor objects with provided ID(s). When summoning gauntlets, bracers or pauldrons, you can provide two IDs.]],
	arguments = {
		{ name = "id",  type = "string", description = "The ID of the armor object to summon." },
		{ name = "id2", type = "string", optional = true, description = "The ID of the additional gauntlet, bracer or pauldron object to summon." },
	},
}
