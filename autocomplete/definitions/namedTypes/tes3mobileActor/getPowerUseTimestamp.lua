return {
	type = "method",
	description = [[Finds the timestamp a recharging power was used. Powers recharge 24 hours after this timestamp. The timestamp units are hours. The current time as a timestamp can be accessed at tes3.getSimulationTimestamp().]],
	arguments = {
		{ name = "power", type = "tes3spell", description = "The spell object for the power." },
	},
	returns = "timestamp",
	valuetype = "number",
}