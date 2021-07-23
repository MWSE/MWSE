return {
	type = "function",
	description = [[This function fetches a reference's attached animation groups.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3reference", description = "A reference whose animation groups to fetch." },
		},
	}},
	returns = "animData",
	valuetype = "table",
}
