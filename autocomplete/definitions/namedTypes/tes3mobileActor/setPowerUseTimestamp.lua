return {
	type = "method",
	description = [[Sets the timestamp a recharging power was used. Powers recharge 24 hours after this timestamp.]],
	arguments = {
		{ name = "power", type = "tes3spell", description = "The spell object for the power." },
		{ name = "timestamp", type = "number", description = "The timestamp of the moment the power was casted, or 24 hours before the recharge point. The timestamp units are hours. The current time as a timestamp can be accessed at tes3.getSimulationTimestamp()." },
	},
}