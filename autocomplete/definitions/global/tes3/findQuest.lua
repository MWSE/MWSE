return {
	type = "function",
	description = [[Finds a journal quest log by dialogue topic or quest name. Pass either a journal dialogue id or a quest name. A quest can cover multiple dialogue journal topics under the same quest name. Quests are also where the flags for active and finished quests are tracked.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "journal", type = "tes3dialogue|string", optional = true, description = "The dialogue journal id to look for." },
			{ name = "name", type = "string", optional = true, description = "The quest name (as displayed in the journal) to look for." },
		},
	}},
	returns = {{ name = "quest", type = "tes3quest?" }},
}
