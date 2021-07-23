return {
	type = "method",
	description = [[Calculates the angle from this reference's current facing to the target reference.]],
	arguments = {
		{ name = "reference", type = "tes3reference", description = "The reference to calculate the angle to." },
	},
	returns = {{ name = "angle", type = "number", description = "The angle to the given reference." }},
}