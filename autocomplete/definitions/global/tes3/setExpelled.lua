return {
	type = "function",
	description = [[This function can expel and undo expelled state for the player in the given faction.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "faction", type = "tes3faction", description = "The faction the player will be expelled from." },
			{ name = "expelled", type = "boolean", optional = true, default = true, description = "Passing `false` will make the player regain membership." },
		},
	}},
}
