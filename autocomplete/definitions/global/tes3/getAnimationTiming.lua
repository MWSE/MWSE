return {
	type = "function",
	description = [[This function fetches a reference's attached animation groups' timings.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3reference|tes3mobileActor|string", description = "A reference whose animation groups' timings to fetch." },
		},
	}},
	returns = "result",
	valuetype = "table",
}
